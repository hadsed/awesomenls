(TeX-add-style-hook "manual"
 (lambda ()
    (LaTeX-add-bibitems
     "fort")
    (TeX-add-symbols
     '("bracket" 2)
     '("ket" 1)
     '("bra" 1))
    (TeX-run-style-hooks
     "amsmath"
     "amssymb"
     "listings"
     "graphicx"
     "graphics"
     "geometry"
     "latex2e"
     "art12"
     "article"
     "12pt")))

