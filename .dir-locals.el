;; https://www.emacswiki.org/emacs/DirectoryVariables

((nil .      
      ((indent-tabs-mode . nil)
	   (tab-width . 4)
	   (fill-column . 80)))
 (c-mode .
         ((c-file-style . "k&r")))
 (c++-mode .
           ((c-file-style . "k&r")))
 (arduino-mode .
               ((compile-command . "arduino-cli compile --fqbn esp32:esp32:esp32 ../cesped")
                (c-file-style . "k&r"))))
