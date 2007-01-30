""" Implements a singleton mixin class """

# The following code was written by Gary Robinson
# (grobinson@transpose.com) and placed in the public domain.  His
# copyright notice is as follows:

# By Gary Robinson, grobinson@transpose.com. No rights reserved -- 
# placed in the public domain -- which is only reasonable considering
# how much it owes to other people's version which are in the
# public domain. The idea of using a metaclass came from 
# a  comment on Gary's blog (see 
# http://www.garyrobinson.net/2004/03/python_singleto.html#comments). 
# Other improvements came from comments and email from other
# people who saw it online. (See the blog post and comments
# for further credits.)

# Not guaranteed to be fit for any particular purpose. Use at your
# own risk.


class SingletonException(Exception):
    pass

class MetaSingleton(type):
    def __new__(metaclass, strName, tupBases, dict):
        if dict.has_key('__new__'):
            raise SingletonException, 'Can not override __new__ in a Singleton'
        return super(MetaSingleton,metaclass).__new__(metaclass, strName, tupBases, dict)
        
    def __call__(cls, *lstArgs, **dictArgs):
        raise SingletonException, 'Singletons may only be instantiated through getInstance()'

class Singleton(object):
    __metaclass__ = MetaSingleton

    def getInstance(cls, *lstArgs):
        """
        Call this to instantiate an instance or retrieve the existing instance.
        If the singleton requires args to be instantiated, include them the first
        time you call getInstance.        
        """
        if cls._isInstantiated():
            if len(lstArgs) != 0:
                raise SingletonException, 'If no supplied args, singleton must already be instantiated, or __init__ must require no args'
        else:
            if cls._getConstructionArgCountNotCountingSelf() > 0 and len(lstArgs) <= 0:
                raise SingletonException, 'If the singleton requires __init__ args, supply them on first instantiation'
            instance = cls.__new__(cls)
            instance.__init__(*lstArgs)
            cls.cInstance = instance
        return cls.cInstance
    getInstance = classmethod(getInstance)
    
    def _isInstantiated(cls):
        return hasattr(cls, 'cInstance')
    _isInstantiated = classmethod(_isInstantiated)  

    def _getConstructionArgCountNotCountingSelf(cls):
        return cls.__init__.im_func.func_code.co_argcount - 1
    _getConstructionArgCountNotCountingSelf = classmethod(_getConstructionArgCountNotCountingSelf)
