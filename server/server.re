open Opium

let () = {
  Logs.set_reporter (Logs_fmt.reporter ());
  Logs.set_level (Some(Logs.Debug));

  let module T = Api.Handlers.Todo(Db);
  let handlers = List.flatten([ Api.Handlers.hello_world, T.endpoints ]);

  App.empty
  |> App.middleware(Middleware.logger)
  |> (initial_app) => List.fold_left((app', handler) => handler(app'), initial_app, handlers)
  |> App.run_command;
}
