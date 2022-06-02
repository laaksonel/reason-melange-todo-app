open Opium

let () =
  Logs.set_reporter (Logs_fmt.reporter ());
  Logs.set_level (Some(Logs.Info));
  App.empty
  |> App.run_command;

