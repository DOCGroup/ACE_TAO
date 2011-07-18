"""A lightweight python templating engine.  Templet version 2 beta.

Supports two templating idioms:
    1. template functions using @stringfunction and @unicodefunction
    2. template classes inheriting from StringTemplate and UnicodeTemplate

Each template function is marked with the attribute @stringfunction
or @unicodefunction.  Template functions will be rewritten to expand
their document string as a template and return the string result.
For example:

    @stringtemplate
    def myTemplate(animal, thing):
      "the $animal jumped over the $thing."

    print myTemplate('cow', 'moon')

The template language understands the following forms:

    $myvar - inserts the value of the variable 'myvar'
    ${...} - evaluates the expression and inserts the result
    ${{...}} - executes enclosed code; use 'out.append(text)' to insert text
    $$ - an escape for a single $
    $ (at the end of the line) - a line continuation

Template functions are compiled into code that accumulates a list of
strings in a local variable 'out', and then returns the concatenation
of them.  If you want do do complicated computation, you can append
to 'out' directly inside a ${{...}} block.

Another alternative is to use template classes.

Each template class is a subclass of StringTemplate or UnicodeTemplate.
Template classes should define a class attribute 'template' that
contains the template code.  Also, any class attribute ending with
'_template' will be compiled into a template method.

Use a template class by instantiating it with a dictionary or
keyword arguments.  Get the expansion by converting the instance
to a string.  For example:

    class MyTemplate(templet.Template):
      template = "the $animal jumped over the $thing."

    print MyTemplate(animal='cow', thing='moon')

Within a template class, the template language is similar to a template
function, but 'self.write' should be used to build the string inside
${{..}} blocks.  Also, there is a shorthand for calling template methods:

    $<sub_template> - shorthand for '${{self.sub_template(vars())}}'

This idiom is helpful for decomposing a template and when subclassing.

A longer example:

    import cgi
    class RecipeTemplate(templet.Template):
      template = r'''
        <html><head><title>$dish</title></head>
        <body>
        $<header_template>
        $<body_template>
        </body></html>
      '''
      header_template = r'''
        <h1>${cgi.escape(dish)}</h1>
      '''
      body_template = r'''
        <ol>
        ${{
          for item in ingredients:
            self.write('<li>', item, '\n')
        }}
        </ol>
      '''

This template can be expanded as follows:

    print RecipeTemplate(dish='burger', ingredients=['bun', 'beef', 'lettuce'])

And it can be subclassed like this:

    class RecipeWithPriceTemplate(RecipeTemplate):
      header_template = "<h1>${cgi.escape(dish)} - $$$price</h1>\n"

Templet is by David Bau and was inspired by Tomer Filiba's Templite class.
For details, see http://davidbau.com/templet

Templet is posted by David Bau under BSD-license terms.

Copyright (c) 2007, David Bau
All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:

    1. Redistributions of source code must retain the above copyright notice,
       this list of conditions and the following disclaimer.

    2. Redistributions in binary form must reproduce the above copyright
       notice, this list of conditions and the following disclaimer in the
       documentation and/or other materials provided with the distribution.

    3. Neither the name of Templet nor the names of its contributors may
       be used to endorse or promote products derived from this software
       without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR
ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
(INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
"""

import sys, re, inspect

class _TemplateBuilder(object):
  __pattern = re.compile(r"""\$(        # Directives begin with a $
        \$                            | # $$ is an escape for $
        [^\S\n]*\n                    | # $\n is a line continuation
        [_a-z][_a-z0-9]*              | # $simple Python identifier
        \{(?!\{)[^\}]*\}              | # ${...} expression to eval
        \{\{.*?\}\}                   | # ${{...}} multiline code to exec
        <[_a-z][_a-z0-9]*>            | # $<sub_template> method call
      )(?:(?:(?<=\}\})|(?<=>))[^\S\n]*\n)? # eat some trailing newlines
    """, re.IGNORECASE | re.VERBOSE | re.DOTALL)

  def __init__(self, constpat, emitpat, callpat=None):
    self.constpat, self.emitpat, self.callpat = constpat, emitpat, callpat

  def __realign(self, str, spaces=''):
    """Removes any leading empty columns of spaces and an initial empty line"""
    lines = str.splitlines();
    if lines and not lines[0].strip(): del lines[0]
    lspace = [len(l) - len(l.lstrip()) for l in lines if l.lstrip()]
    margin = len(lspace) and min(lspace)
    return '\n'.join((spaces + l[margin:]) for l in lines)

  def build(self, template, filename, s=''):
    code = []
    for i, part in enumerate(self.__pattern.split(self.__realign(template))):
      if i % 2 == 0:
        if part: code.append(s + self.constpat % repr(part))
      else:
        if not part or (part.startswith('<') and self.callpat is None):
          raise SyntaxError('Unescaped $ in ' + filename)
        elif part.endswith('\n'): continue
        elif part == '$': code.append(s + self.emitpat % '"$"')
        elif part.startswith('{{'): code.append(self.__realign(part[2:-2], s))
        elif part.startswith('{'): code.append(s + self.emitpat % part[1:-1])
        elif part.startswith('<'): code.append(s + self.callpat % part[1:-1])
        else: code.append(s + self.emitpat % part)
    return '\n'.join(code)

class _TemplateMetaClass(type):
  __builder = _TemplateBuilder(
    'self.out.append(%s)', 'self.write(%s)', 'self.%s(vars())')

  def __compile(cls, template, n):
    globals = sys.modules[cls.__module__].__dict__
    if '__file__' not in globals: filename = '<%s %s>' % (cls.__name__, n)
    else: filename = '%s: <%s %s>' % (globals['__file__'], cls.__name__, n)
    code = compile(cls.__builder.build(template, filename), filename, 'exec')
    def expand(self, __dict = None, **kw):
      if __dict: kw.update([i for i in __dict.iteritems() if i[0] not in kw])
      kw['self'] = self
      exec code in globals, kw
    return expand

  def __init__(cls, *args):
    for attr, val in cls.__dict__.items():
      if attr == 'template' or attr.endswith('_template'):
        if isinstance(val, basestring):
          setattr(cls, attr, cls.__compile(val, attr))
    type.__init__(cls, *args)

class StringTemplate(object):
  """A base class for string template classes."""
  __metaclass__ = _TemplateMetaClass

  def __init__(self, *args, **kw):
    self.out = []
    self.template(*args, **kw)

  def write(self, *args):
    self.out.extend([str(a) for a in args])

  def __str__(self):
    return ''.join(self.out)

# The original version of templet called StringTemplate "Template"
Template = StringTemplate

class UnicodeTemplate(object):
  """A base class for unicode template classes."""
  __metaclass__ = _TemplateMetaClass

  def __init__(self, *args, **kw):
    self.out = []
    self.template(*args, **kw)

  def write(self, *args):
    self.out.extend([unicode(a) for a in args])

  def __unicode__(self):
    return u''.join(self.out)

  def __str__(self):
    return unicode(self).encode('utf-8')

def _templatefunction(func, listname, stringtype):
  globals, locals = sys.modules[func.__module__].__dict__, {}
  if '__file__' not in globals: filename = '<%s>' % func.__name__
  else: filename = '%s: <%s>' % (globals['__file__'], func.__name__)
  builder = _TemplateBuilder('%s.append(%%s)' % listname,
                             '%s.append(%s(%%s))' % (listname, stringtype))
  args = inspect.getargspec(func)
  code = [
    'def %s%s:' % (func.__name__, inspect.formatargspec(*args)),
    ' %s = []' % listname,
    builder.build(func.__doc__, filename, ' '),
    ' return "".join(%s)' % listname]
  code = compile('\n'.join(code), filename, 'exec')
  exec code in globals, locals
  return locals[func.__name__]

def stringfunction(func):
  """Function attribute for string template functions"""
  return _templatefunction(func, listname='out', stringtype='str')

def unicodefunction(func):
  """Function attribute for unicode template functions"""
  return _templatefunction(func, listname='out', stringtype='unicode')

# When executed as a script, run some testing code.
if __name__ == '__main__':
  ok = True
  def expect(actual, expected):
    global ok
    if expected != actual:
      print "error - got:\n%s" % repr(actual)
      ok = False
  class TestAll(Template):
    """A test of all the $ forms"""
    template = r"""
      Bought: $count ${name}s$
       at $$$price.
      ${{
        for i in xrange(count):
          self.write(TestCalls(vars()), "\n")  # inherit all the local $vars
      }}
      Total: $$${"%.2f" % (count * price)}
    """
  class TestCalls(Template):
    """A recursive test"""
    template = "$name$i ${*[TestCalls(name=name[0], i=n) for n in xrange(i)]}"
  expect(
    str(TestAll(count=5, name="template call", price=1.23)),
    "Bought: 5 template calls at $1.23.\n"
    "template call0 \n"
    "template call1 t0 \n"
    "template call2 t0 t1 t0 \n"
    "template call3 t0 t1 t0 t2 t0 t1 t0 \n"
    "template call4 t0 t1 t0 t2 t0 t1 t0 t3 t0 t1 t0 t2 t0 t1 t0 \n"
    "Total: $6.15\n")
  class TestBase(Template):
    template = r"""
      <head>$<head_template></head>
      <body>$<body_template></body>
    """
  class TestDerived(TestBase):
    head_template = "<title>$name</title>"
    body_template = "${TestAll(vars())}"
  expect(
    str(TestDerived(count=4, name="template call", price=2.88)),
    "<head><title>template call</title></head>\n"
    "<body>"
    "Bought: 4 template calls at $2.88.\n"
    "template call0 \n"
    "template call1 t0 \n"
    "template call2 t0 t1 t0 \n"
    "template call3 t0 t1 t0 t2 t0 t1 t0 \n"
    "Total: $11.52\n"
    "</body>\n")
  class TestUnicode(UnicodeTemplate):
    template = u"""
      \N{Greek Small Letter Pi} = $pi
    """
  expect(
    unicode(TestUnicode(pi = 3.14)),
    u"\N{Greek Small Letter Pi} = 3.14\n")
  goterror = False
  try:
    class TestError(Template):
      template = 'Cost of an error: $0'
  except SyntaxError:
    goterror = True
  if not goterror:
    print 'TestError failed'
    ok = False
  @stringfunction
  def testBasic(name):
    "Hello $name."
  expect(testBasic('Henry'), "Hello Henry.")
  @stringfunction
  def testReps(a, count=5): r"""
    ${{ if count == 0: return '' }}
    $a${testReps(a, count - 1)}"""
  expect(
    testReps('foo'),
    "foofoofoofoofoo")
  @unicodefunction
  def testUnicode(count=4): u"""
    ${{ if not count: return '' }}
    \N{BLACK STAR}${testUnicode(count - 1)}"""
  expect(
    testUnicode(count=10),
    u"\N{BLACK STAR}" * 10)
  if ok: print "OK"
