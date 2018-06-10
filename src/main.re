open Belt;

exception ArgumentParseError(option(string));
exception Unhandled;

let compile = (~from, ~to_, ~is_debug=false, ()) => {
  let source_file = Loc.SourceFile(from);
  let source_content = Node.Fs.readFileSync(from, `utf8);
  let (ast, _) =
    Parser_flow.program_file(source_content, Some(source_file));
  let (_, statements, _) = ast;
  let ts =
    try (statements
    |> List.map(_, Translate.statement)
    |> List.toArray
    |> Js.Array.joinWith("\n")) {
      | Translate.Yet.Error(e) => Translate.Yet.string_of_t(e)
    };
    
  if (!is_debug) {
    Node.Fs.writeFileSync(to_, ts, `utf8);
  };
  ts;
};

let parse_argv: list(string) => string =
  fun
  | ["-v", ..._]
  | ["--version", ..._] => "exodus 0.0.1"
  | ["-h", ..._]
  | ["--help", ..._] => "
exodus 0.0.1
Convert Flow to TypeScript

USAGE: exodus [FROM_FILE] [TO_FILE]
"
  | [from, to_] => compile(~from, ~to_, ())
  | [from, to_, "-d"]
  | [from, to_, "--debug"] => compile(~from, ~to_, ~is_debug=true, ())
  | x => raise(ArgumentParseError(Js.Json.stringifyAny(x)));

let () =
  Sys.argv
  |> List.fromArray
  |> (
    argv =>
      switch (argv) {
      | []
      | [_]
      | [_, _] => raise(Unhandled)
      | [_, _, ...argv] => argv
      }
  )
  |> parse_argv
  |> Js.log
  ;
