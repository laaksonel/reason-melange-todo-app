open Opium

let () = {
  Logs.set_reporter (Logs_fmt.reporter ());
  Logs.set_level (Some(Logs.Info));

  App.empty
  |> App.middleware(Middleware.logger)
  |> Db.middleware
  |> (initial_app) => List.fold_left((app', handler) => handler(app'), initial_app, Api.Handlers.handlers)
  |> App.run_command;
}
