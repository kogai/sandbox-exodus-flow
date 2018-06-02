open Parser_flow
let source = "
// @flow

export const f: string => number = (x) => {
  return Number(x)
};
"

let () = 
  let content = Some (File_key.SourceFile "sample.js") in
  let (ast, _) = program_file source content in
  print_endline source;

