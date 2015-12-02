ACE/TAO/CIAO/DAnCE
==================

See [Douglas C. Schmidt website](http://www.dre.vanderbilt.edu/~schmidt)  for more information about ACE/TAO/CIAO/DAnCE (ATCD for short)

The quality of ATCD is monitored through our distributed [scoreboard](http://www.dre.vanderbilt.edu/scoreboard/)


Travis-CI status
================

master: [![Build Status](https://travis-ci.org/DOCGroup/ATCD.svg?branch=master)](https://travis-ci.org/DOCGroup/ATCD)

Snap-CI status
================

master: [![Build Status](https://snap-ci.com/DOCGroup/ATCD/branch/master/build_image)](https://snap-ci.com/DOCGroup/ATCD/branch/master)

AppVeyor status
================

master: [![Build Status](https://ci.appveyor.com/api/projects/status/uxnvychqk0tmy3sh/branch/master?svg=true)](https://ci.appveyor.com/project/jwillemsen/atcd)

Coverity Scan status
====================

master: [![Coverity Scan Build Status](https://scan.coverity.com/projects/1/badge.svg)](https://scan.coverity.com/projects/1)

<!---
GitHub does not support styles on HTML: https://github.com/github/markup/issues/57 so just
use icons from...
http://p.yusukekamiyamane.com/
http://www.iconarchive.com/show/fugue-icons-by-yusuke-kamiyamane.html
--->

[tip]: http://icons.iconarchive.com/icons/yusuke-kamiyamane/fugue/16/tick-circle-frame-icon.png "Tip"
[warn]: http://icons.iconarchive.com/icons/yusuke-kamiyamane/fugue/16/exclamation-icon.png "Warning"
[err]: http://icons.iconarchive.com/icons/yusuke-kamiyamane/fugue/16/minus-circle-icon.png "Error"

# ATCD #

ATCD is a [freely licensed](https://raw.githubusercontent.com/DOCGroup/ATCD/master/ACE/COPYING) C++ suite. [ACE](http://www.dre.vanderbilt.edu/~schmidt/ACE.html) is a C++ framework for implementing distributed and networked applications, [TAO](http://www.dre.vanderbilt.edu/~schmidt/TAO.html) is a C++ implementation of the OMG's [CORBA](http://www.omg.org/spec/CORBA/) standard, [CIAO](http://www.dre.vanderbilt.edu/~schmidt/CIAO.html) is a C++ implementation of the OMG's [LwCCM](http://www.omg.org/spec/CORBA/) standard, and DAnCE is a C++ implementation of the OMG's [Deployment and Configuration](http://www.omg.org/spec/DEPL/) standard. The main home page is [here](http://www.dre.vanderbilt.edu/~schmidt/). Mailing lists are available [here](http://www.dre.vanderbilt.edu/~schmidt/ACE-mail.html) and the Bugzilla for issues is [here](http://bugzilla.dre.vanderbilt.edu/).

## Contributing Via GitHub ##

These guidelines are for all developers, whether occasional or regular. They have been sourced from various 'best practice' documents but primarily [here](https://github.com/JacORB/JacORB).

![Warning][warn] As a convention, <i>upstream</i> is used as the name of the <a href="http://github.com/DOCGroup/ATCD">http://github.com/DOCGroup/ATCD</a> repository. This repository is the canonical repository for ATCD. We usually name <i>origin</i> the fork on github of each contributor. So the exact meaning of <i>origin</i> is relative to the developer: you could think of <i>origin</i> as your own fork.

## Pre-requisites

This document assumes some working knowledge of git. We recommend Scott Chacon's excellent [Pro Git](http://progit.org) a a valuable piece of background reading. The book is released under the Creative Commons license and can be downloaded in electronic form for free. At very least, we recommend that [Chapter 2](http://progit.org/book/ch2-0.html), [Chapter 3](http://progit.org/book/ch3-0.html) and [Chapter 5](http://progit.org/book/ch5-0.html) of Pro Git are read before proceeding.

## Repositories

ATCD uses [http://github.com/DOCGroup/ATCD](http://github.com/DOCGroup/ATCD) as its canonical repository, and this repository contains the stable code on master.

Typically, only _Project Admins_ would be able to push to this repo while all else may clone or fork this repo.

## Roles

The project assumes one of 3 _roles_ an individual may assume when interacting with the ATCD codebase. The three roles here are:

*  _Project Admin_
*  _Frequent Contributor_
*  _Occasional Contributor_

![Warning][warn] Typically, someone may be in more than one role at any given time, and puts on a different "hats" based on the task at hand


### Occasional Contributor

This role defines a user who browses through the source code of the project and occasionally submits patches. Patches may be submitted in one of two ways:

*  Attach a patch file to the Bugzila issue (ATCD Bug tracking system may be found [here](http://bugzilla.dre.vanderbilt.edu/)).
*  Creating a pull request on GitHub

The approach a contributor chooses to use depends entirely his/her personal preference, but usually is tied to how the contributor accesses ATCD's source code. If the contributor directly clones the upstream repository, they should submit patch files. If the contributor instead uses their personal GitHub account to fork the upstream repository, then they are should issue a pull request.

![Tip][tip] A GitHub pull request is the preferred method to submit a patch!


#### Attach a patch file to the Bugzilla issue

![Git Workflow 1](https://docs.jboss.org/author/download/attachments/4784485/git_wf_1.png)

In this workflow, the contributor directly clones the upstream repository, makes changes to his local clone, adequately tests and commits his work with proper comments (more on commit comments later), and generates a patch. The patch should then be attached to the Bugzilla issue.

More information on generating patches suitable for attaching to a Bugzilla can be found in [Chapter 5, Section 2](http://progit.org/book/ch5-2.html) of Pro Git, under the section titled *Public Large Project*.

![Warning][warn] Rather than emailing the patches to a developer mail list, please attach your patch to the Bugzilla issue.


#### Creating a pull request on GitHub

![Git Workflow 2](https://docs.jboss.org/author/download/attachments/4784485/git_wf_2.png)

In this workflow, the contributor forks the ATCD upstream repository on GitHub, clones their fork, and makes changes to this private fork. When changes have been tested and are ready to be contributed back to the project, a *pull request* is issued via GitHub so that one of the Project Administrators can pull in the change.

![Warning][warn] It is desirable to work off a <i>topic branch</i>, even when using your own, forked repository. A topic branch is created for every feature or bug fix you do. Typically you would create one topic branch per issue, but if several patches are related it's acceptable to have several commits in the same branch; however different changes should always be identified by different commits.


Before you push your work onto your fork of the repository, it is often a good idea to review your commits. Consolidating them (squashing) or breaking them up as necessary and cleaning up commit messages should all be done while still working off your local clone. Also, prior to issuing a pull request, you should make sure you rebase your branch against the upstream branch you expect it to be merged into.  Also, only submit pull requests for your branch - not for your master!

The section on *Public Small Project* in [Chapter 5, Section 2](http://progit.org/book/ch5-2.html) of Pro Git has more information on this style of workflow.

##### A worked example

1. Make sure your master is synced up with upstream. See [this section](#if-you-have-forked-upstream) for how to do this
2. Create new branch for your topic and switch to it. For the example issue, BZ-1234:

```
git checkout -b BZ-12345 master
```

3. Do your work. Test. Repeat
4. Commit your work on your topic branch
5. Push your topic branch to GitHub. For example:

```
git push origin BZ-12345
```

6. Issue a pull request using the [GitHub pull request system](http://help.github.com/send-pull-requests)
7. Once your pull request has been applied upstream, delete the topic branch both locally and on your fork. For example:

```
git branch -d BZ-12345 && git push origin :BZ-12345
```

8. Sync with upstream again so that your changes now appear in your master branch

If your topic branch has been open for a while and you are afraid changes upstream may clash with your changes, it makes sense to rebase your topic branch before you issue a pull request. To do this:

1. Sync your master branch with upstream

```
git checkout master
git pull upstream master
```

2. Switch to your topic branch. For example:

```
git checkout BZ-12345
```

3. Rebase your topic branch against master:

```
git rebase master
```

4. During the rebase process you might need to fix conflicts;
5. when you're done test your code again.
6. Push your rebased topic branch to your repo on GitHub (you will likely need to force this with the _-f_ option).

```
git push -f origin BZ-12345
```

7. Continue your work on your topic branch.

![Warning][warn] If you are sharing your forked ATCD repo with others, then do not rebase! Use a merge instead.


#### Multi-step coordination between developers using forked repositories

Sometimes a feature/task is rather complex to implement and requires competence from multiple areas of the projects. In such occasions it is not uncommon for developers to coordinate feature implementation using personal forks of ATCD prior to finally issuing request to integrate into ATCD main repository on GitHub.

For example, developer A using his personal ATCD fork creates a topic branch T and completes as much work as he/she can before requesting for assistance from developer B. Developer A pushes topic T to his personal ATCD fork where developer B picks it up and brings it down to his local repo. Developer B then in turn completes necessary work, commits his/her changes on branch T, and finally pushes back T to his own personal fork. After issuing request for pull to developer A, developer B waits for notification that developer A integrated his changes. This exchange can be repeated as much as it is necessary and can involve multiple developers.

##### A worked example

This example assumes that developer A and B have added each others ATCD forked repositories with the *git add remote* command. For example, developer B would add developer A's personal ATCD fork repository with the command

```
    git remote add devA https://github.com/developerA/ATCD.git
```

1. Developer A starts implementing feature BZ-244 and works on a local topic branch `bz244`
Developer A pushes `bz244` to personal ATCD fork. For example:

```
    git push origin bz244
```

2. Developer B fetches branch `bz244` to local repository. For example:

```
    git fetch devA bz244:my_bz244
```

3. Developer B works on local branch `my_bz244`
4. Developer B commits changes, pushes `my_bz244` to own fork.

```
    git push origin my_bz244
```

5. Developer B sends pull request to developer A to integrate changes from `my_bz244` to `bz244`

### Frequent Contributor

A frequent contributor will only ever submit patches via a pull requests. The pull request will be submitted via GitHub.

Frequent contributors should *always* fork the upstream project on GitHub and work off a clone of this fork. This is very similar to [Creating a pull request on GitHub](#creating-a-pull-request-on-github) workflow used by a [#Occasional Contributor](#occasional-contributor).

![Warning][warn] All ATCD core developers are considered frequent contributors and work off personal forks of the upstream repository. This allows for complex features to be developed in parallel without tripping up over one another. This process is certainly not restricted to just ATCD core developers; any contributor is welcome to also participate in this manner.



### Project Admin

Project Admins have a very limited role. Only Project Admins are allowed to push to upstream, and Project Admins _never_ write any code directly on the upstream repository. All Project Admins do is pull in and merge changes from contributors (even if the "contributor" happens to be themselves) into upstream, perform code reviews and either commit or reject such changes.

![Warning][warn] All Contributors who are also Project Admins are encouraged to not merge their own changes, to ensure that all changes are reviewed.


This approach ensures ATCD maintains quality on the main code source tree, and allows for important code reviews to take place again ensuring quality. Further, it ensures clean and easily traceable code history and makes sure that more than one person knows about the changes being performed.

#### Merging in patches

![Git Workflow 3](https://docs.jboss.org/author/download/attachments/4784485/git_wf_3.png)

Patches submitted via Bugzilla are audited and promoted to the upstream repository as detailed above. A Project Admin would typically create a working branch to which the patch is applied and tested. The patch can be further modified, cleaned up, and commit messages made clearer if necessary. The branch should then be merged to the master or one of the maintenance branches before being pushed.

More information on applying patches can be found in [Chapter 5, Section 3](http://progit.org/book/ch5-3.html) of Pro Git, under *Applying Patches From Email.*

#### Handling pull requests

![Git Workflow 4](https://docs.jboss.org/author/download/attachments/4784485/git_wf_4.png)

Project Admins are also responsible for responding to pull requests. The process is similar to applying a patch directly, except that when pulling in changes from a forked repository, more than a single commit may be pulled in. Again, this should be done on a newly created working branch, code reviewed, tested and cleaned up as necessary.

If commits need to be altered - e.g., rebasing to squash or split commits, or to alter commit messages - it is often better to contact the Contributor and ask the Contributor to do so and re-issue the pull request, since doing so on the upstream repo could cause update issues for contributors later on. If commits were altered or three-way merge was performed during a merge instead of fast-forward, it's also a good idea to check the log to make sure that the resulting repository history looks OK:

```
$ git log --pretty=oneline --graph --abbrev-commit  # History messed up due to a bad merge
*   3005020 Merge branch 'BZ-786' of git://github.com/rnc/ATCD
|\
| * e757265 BZ-786 Make dependency to foobar optional  <-- Same with cb4e5d6 - unnecessary
* | cb4e5d6 BZ-786 Make dependency to foobar optional  <-- Cherry-picked commit by other admin
|/
* ...

$ git reset cb4e5d6  # revert the bad merge
```

It is therefore __strongly recommended__ that you use the [handle_pull_request](https://github.com/maniksurtani/githelpers/blob/master/project_admins/handle_pull_request) script that ensures a clean merge. If you _still_ wish to do this manually, please consider reading through the script first to get an idea of what needs to happen.

More information on pulling changes from remote, forked repos can be found in [Chapter 5, Section 3](http://progit.org/book/ch5-3.html) of Pro Git, under *Checking Out Remote Branches* .

##### Possible trouble handling pull requests

*  If you have warnings about "Merge made by recursive" you have to fix it rebasing.
*  If you have warnings about "non-fast-forward" you have to rebase.
*  If you see "non-fast-forward updates were rejected" you shall never use "force" on upstream! It means that another patch was merged before you and you have to update your master again, and rebase again.
*  "force" is allowed only in special maintenance circumstances. If you find you're needing it to handle a pull request, then you're doing it wrong, and the mistake might be a dangerous one! It's like the good rule of never commit when you're drunk (coding is allowed).

![Error][err] __Never use force on git push__ Using _-f_ while pushing on a shared repository such as _upstream_ you could effectively erase other committed patches. No-one shall ever use this option unless unanimously approved on the public mailing list: the most dangerous aspect of it is that nobody gets any notification if this happens, and we might think issues are solved but you silently removed the fix and it's history from the repository.


#### Cutting releases

Releases can only be cut by Project Admins, and must be done off a recently updated (git fetch and git pull origin) clone of the upstream repo.

## Committing your work

### Release branches

ATCD currently has a primary release branch of master and a maintenance branch used by Red Hat JBoss. In future further release branches may be created. (e.g. for maintenance purposes). Work should never be committed directly to a release branch; topic branches should always be used for work, and these topic branches should be merged in.

### Topic branches

Some of the biggest features of git are speed and efficiency of branching, and accuracy of merging. As a result, best practices involve making frequent use of branches. Creating several topic branches a day, even, should not be considered excessive, and working on several topic branches simultaneously again should be commonplace.

[Chapter 3, Section 4](http://progit.org/book/ch3-4.html) of Pro Git has a detailed discussion of topic branches. For ATCD, it makes sense to create a topic branch and name it after the Bugzilla it corresponds to. (if it doesn't correspond to a Bugzilla, a simple but descriptive name should be used).

#### Topic Branches Affecting More Than One Release Branch

Most topic branches will only affect the a single release branch, so a topic branch should be created based off master. It may be that fixes apply to multiple branches. In this case, the following workflow should apply:

* Create topic branch off maintenance branch e.g. 6.3.1. For example:
```git checkout -b <topic>_ACE+TAO+CIAO-6_3_1 ACE+TAO+CIAO-6_3_1```
* Create topic branch off master. For example:
```git checkout -b <topic>_master master```
* Do your work on `<topic>_master`, test and commit your fixes
* Switch to `<topic>_ACE+TAO+CIAO-6_3_1`. For example:
```git checkout <topic>_ACE+TAO+CIAO-6_3_1```
* Cherry-pick your commit on `<topic>_master` onto `<topic>_ACE+TAO+CIAO-6_3_1`. For example:
```git cherry-pick <commit_id>```
* Test `<topic>_ACE+TAO+CIAO-6_3_1` for correctness, modify as necessary
* Issue pull requests for both topic branches

### Comments

It is __extremely important__ that comments for each commit are clear and follow certain conventions. This allows for proper parsing of logs by git tools. Read [this article](http://tbaggery.com/2008/04/19/a-note-about-git-commit-messages.html) on how to format comments for git and adhere to them. Further to the recommendations in the article, the short summary of the commit message should be in the following format:

    BZ-XXX Subject line of the Bugzilla in question

This can optionally be followed by a detailed explanation of the commit. Why it was done, how much of it was completed, etc. You may wish to express this as a list, for example:

```
* Add a unit test
* Add more unit tests
* Fix regressions
* Solve major NP-Complete problems
```

Make sure however to split separate concerns - especially if they are unrelated - in separate commits.

### Commits

Sometimes work on your topic branch may include several commits. For example, committing a test. Then committing another test. Then perhaps committing a fix. And perhaps fixing your own fix in the next commit... Before issuing a pull request for this topic branch, consider cleaning up these commits. Interactive rebasing helps you squash several commits into a single commit, which is often more coherent to deal with for others merging in your work. [Chapter 6, Section 4](http://progit.org/book/ch6-4.html) of Pro Git has details on how to squash commits and generally, clean up a series of commits before sharing this work with others. Note that you can also easily reorder them, just change the order of lines during the interactive rebase process.

Also, it is important to make sure you don't accidentally commit files for which no real changes have happened, but rather, whitespace has been modified. This often happens with some IDEs. `git diff --check` should be run before you issue such a pull request, which will check for such "noise" commits and warn you accordingly. Such files should be reverted and not be committed to the branch.

Adhering to [ATCD's code style](http://htmlpreview.github.io/?https://github.com/DOCGroup/ATCD/blob/master/ACE/docs/ACE-guidelines.html) guidelines will help minimise "noise" commits. Project Admins are going to ask contributors to reformat their code if necessary.

## Keeping your repo in sync with upstream

### If you have cloned upstream

If you have a clone of the upstream, you may want to update it from time to time. Running:

```
$ git fetch origin
$ git fetch origin --tags
```

will often do the trick. You could then pull the specific branches you would need to update:

```
$ git checkout master
$ git pull origin master
$ git checkout ACE+TAO+CIAO-6_3_1
$ git pull origin ACE+TAO+CIAO-6_3_1
```


#### Updating topic branches

You should rebase your topic branches at this point so that they are up-to-date and when pulled by upstream, upstream can fast-forward the release branches:
```
$ git checkout <topic>_master
$ git rebase master
```

and/or

```
$ git checkout topic_ACE+TAO+CIAO-6_3_1
$ git rebase ACE+TAO+CIAO-6_3_1
```

### If you have forked upstream

If you have a fork of upstream, you should probably define upstream as one of your remotes:

```
$ git remote add upstream git://github.com/DOCGroup/ATCD.git
```

You should now be able to fetch and pull changes from upstream into your local repository, though you should make sure you have no uncommitted changes. (You *do* use topic branches, right?)

```
$ git fetch upstream
$ git fetch upstream --tags
$ git checkout master
$ git pull upstream master
$ git push origin master
$ git checkout ACE+TAO+CIAO-6_3_1
$ git pull upstream ACE+TAO+CIAO-6_3_1
$ git push origin ACE+TAO+CIAO-6_3_1
```

A script can do this for you - have a look at [sync_with_upstream](https://github.com/maniksurtani/githelpers/blob/master/contributors/sync_with_upstream).

#### Updating topic branches

Again, you should rebase your topic branches at this point so that they are up-to-date and when pulled by upstream, upstream can fast-forward the release branches:

```
$ git checkout topic_master
$ git rebase master
```

and/or

```
$ git checkout topic_ACE+TAO+CIAO-6_3_1
$ git rebase ACE+TAO+CIAO-6_3_1
```

The sync_with_upstream script can do this for you if your topic branch naming conventions match the script.

## GIT Enhancements

There are many ways git usage may be enhanced:
*  Autocompletion on the command line via Bash or ZSH
*  Custom shell prompts to display GIT repository information (again, available in Bash & ZSH)
*  Coloring GIT Terminal output
*  Visual tools such as gitk, qgit, Eclipse integration, etc.
*  etc.


<br/>
<br/>
<br/>
<br/>
<br/>
<br/>
<br/>
<br/>
<footer>
<p><i>Some icons by <a href="http://p.yusukekamiyamane.com">Yusuke Kamiyamane</a></i></p>
</footer>
