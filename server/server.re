open Opium

let echo_path_param = req => {
  let param = Router.param(req, "param");
  Logs.info(m => m("Called %s", param));
  Response.of_plain_text(param) |> Lwt.return;
}

let () = {
  Logs.set_reporter (Logs_fmt.reporter ());
  Logs.set_level (Some(Logs.Info));

  App.empty
  |> App.get("/:param", echo_path_param)
  |> App.run_command;
}
