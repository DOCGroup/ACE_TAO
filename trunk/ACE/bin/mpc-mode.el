;;; mpc-mode.el --- Makefile Project Creator mode for Emacs

;; Author: Jules Colding  <colding@42tools.com>
;; Maintainer: Jules Colding
;; Keywords: languages, faces, mpc

;; Copyright (C) 2008 Jules Colding <colding@42tools.com>
;;
;; This program is free software: you can redistribute it and/or modify
;; it under the terms of the GNU General Public License as published by
;; the Free Software Foundation, either version 3 of the License, or
;; (at your option) any later version.
;;
;; This program is distributed in the hope that it will be useful,
;; but WITHOUT ANY WARRANTY; without even the implied warranty of
;; MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
;; GNU General Public License for more details.
;;
;; You should have received a copy of the GNU General Public License
;; along with this program.  If not, see <http://www.gnu.org/licenses/>.

;; A major mode for editing MPC input. Please see:
;;
;;   http://downloads.ociweb.com/MPC/MakeProjectCreator.pdf
;;
;; Derived from autoconf-mode.el by Martin Buchholz (martin@xemacs.org)

;;; Your should add the following to your Emacs configuration file:

;; (autoload 'mpc-mode "mpc-mode"
;;   "Major mode for editing MPC files." t)
;; (setq auto-mode-alist (cons '("\\.mwb$" . mpc-mode) auto-mode-alist))
;; (setq auto-mode-alist (cons '("\\.mwc$" . mpc-mode) auto-mode-alist))
;; (setq auto-mode-alist (cons '("\\.mpb$" . mpc-mode) auto-mode-alist))
;; (setq auto-mode-alist (cons '("\\.mpc$" . mpc-mode) auto-mode-alist))

;;; Code:

(require 'font-lock)

(defvar mpc-font-lock-keywords
  `(
    ("\\(project\\)"              1 font-lock-warning-face t)
    ("\\(workspace\\)"            1 font-lock-warning-face t)
    ("(\\([^()]*\\))"             1 font-lock-constant-face t)
    ("\\(IDL_Files\\)"            1 font-lock-keyword-face t) 
    ("\\(Source_Files\\)"         1 font-lock-keyword-face t)
    ("\\(Header_Files\\)"         1 font-lock-keyword-face t)
    ("\\(Inline_Files\\)"         1 font-lock-keyword-face t)
    ("\\(Template_Files\\)"       1 font-lock-keyword-face t)
    ("\\(Resource_Files\\)"       1 font-lock-keyword-face t)
    ("\\(Pkgconfig_Files\\)"      1 font-lock-keyword-face t)
    ("\\(exclude\\)"              1 font-lock-type-face t)
    ("\\(custom_only\\)"          1 font-lock-type-face t)
    ("\\(cmdline\\)"              1 font-lock-type-face t)
    ("\\(avoids\\)"               1 font-lock-type-face t)
    ("\\(exename\\)"              1 font-lock-type-face t)
    ("\\(install =\\)"            1 font-lock-type-face t)
    ("\\(install +=\\)"           1 font-lock-type-face t)
    ("\\(install -=\\)"           1 font-lock-type-face t)
    ("\\(libs\\)"                 1 font-lock-type-face t)
    ("\\(macros\\)"               1 font-lock-type-face t)
    ("\\(after\\)"                1 font-lock-type-face t)
    ("\\(libout\\)"               1 font-lock-type-face t)
    ("\\(libpaths\\)"             1 font-lock-type-face t)
    ("\\(includes\\)"             1 font-lock-type-face t)
    ("\\(automatic\\)"            1 font-lock-type-face t)
    ("\\(command\\)"              1 font-lock-type-face t)
    ("\\(output_option\\)"        1 font-lock-type-face t)
    ("\\(header_pre_extension\\)" 1 font-lock-type-face t)
    ("\\(header_outputext\\)"     1 font-lock-type-face t)
    ("\\(sharedname\\)"           1 font-lock-type-face t)
    ("\\(dynamicflags\\)"         1 font-lock-type-face t)
    ("\\(idlflags\\)"             1 font-lock-type-face t)
    ("\\(:\\)"                    1 font-lock-builtin-face t)
    ("\\( = \\)"                  1 font-lock-builtin-face t)
    ("\\(+=\\)"                   1 font-lock-builtin-face t)
    ("\\(-=\\)"                   1 font-lock-builtin-face t)
    ("\\(//\\)"                   1 font-lock-comment-face t)
    ("\\//\\(.*\\)"               1 font-lock-comment-face t)
    "default font-lock-keywords")
)

(defvar mpc-mode-syntax-table nil
  "syntax table used in mpc mode")
(setq mpc-mode-syntax-table (make-syntax-table))
;;(modify-syntax-entry ?\" "\""  mpc-mode-syntax-table)
;;(modify-syntax-entry ?\' "\""  mpc-mode-syntax-table)
;;(modify-syntax-entry ?#  "<\n" mpc-mode-syntax-table)
;;(modify-syntax-entry ?\n ">#"  mpc-mode-syntax-table)
;;(modify-syntax-entry ?\( "()"   mpc-mode-syntax-table)
;;(modify-syntax-entry ?\) ")("   mpc-mode-syntax-table)
;;(modify-syntax-entry ?\[ "(]"  mpc-mode-syntax-table)
;;(modify-syntax-entry ?\] ")["  mpc-mode-syntax-table)
;;(modify-syntax-entry ?*  "."   mpc-mode-syntax-table)
;;(modify-syntax-entry ?_  "_"   mpc-mode-syntax-table)

(defvar mpc-mode-map
  (let ((map (make-sparse-keymap)))
    (define-key map '[(control c) (control c)] 'comment-region)
    map))

;;;###autoload
(defun mpc-mode ()
  "A major-mode to edit MPC files.
\\{mpc-mode-map}
"
  (interactive)
  (kill-all-local-variables)
  (use-local-map mpc-mode-map)

  (make-local-variable 'comment-start)
  (setq comment-start "//")
  (make-local-variable 'parse-sexp-ignore-comments)
  (setq parse-sexp-ignore-comments t)

  (make-local-variable	'font-lock-defaults)
  (setq major-mode 'mpc-mode)
  (setq mode-name "MPC")

  (setq font-lock-defaults `(mpc-font-lock-keywords nil t))
  
  (set-syntax-table mpc-mode-syntax-table)
  (run-hooks 'mpc-mode-hook)
)

(provide 'mpc-mode)

;;; mpc-mode.el ends here
