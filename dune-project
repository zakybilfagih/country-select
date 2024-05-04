(lang dune 3.15)

(generate_opam_files)

(using melange 0.1)

(package
 (allow_empty)
 (name country-select)
 (depends
  (ocaml
   (>= "5.1.1"))
  (reason
   (>= "3.11.0"))
  (melange
   (>= "3.0.0-51"))
  (reason-react
   (>= "0.14.0"))
  (styled-ppx
   (>= "0.56.0"))
  ppx_deriving_json
  melange-fetch
  (ocaml-lsp-server :with-dev-setup)))
