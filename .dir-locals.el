;; https://www.emacswiki.org/emacs/DirectoryVariables

((nil .
      ((indent-tabs-mode . nil)
       (tab-width . 4)
       (fill-column . 80)))
 (c++-mode .      
           ((fqbn . "esp32:esp32:esp32")
            (c-file-style . "k&r")
            (compile-command . (concat "arduino-cli compile --fqbn " fqbn " " (file-name-directory (buffer-file-name))))))
 (arduino-mode .
               ((fqbn . "esp32:esp32:esp32")
                (c-file-style . "BSD")
                (compile-command . (concat "arduino-cli compile --fqbn " fqbn " " (file-name-directory (buffer-file-name)))))))
