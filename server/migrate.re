open Cmdliner;
open Core;

let ( / ) = Filename.concat
let base_dir = Filename.current_dir_name / "db" / "migrate";
let up_dir = base_dir / "up";
let down_dir = base_dir / "down";

let get_migrations () = Sys_unix.ls_dir(up_dir);

let make_pool = () => {
  let connection_uri = Printf.sprintf("postgresql://postgres:postgres@127.0.0.1:5432/todo");
  connection_uri |> Uri.of_string |> Caqti_lwt.connect_pool(~max_size=10)
  |> fun
  | Ok(pool) => pool
  | Error(err) => failwith((Caqti_error.show(err)));
}

let get_last_migration =
  [%rapper get_opt(
      {sql|
      SELECT @string{migration}, @bool{up}, @ptime{applied}
      FROM schema_migrations
      ORDER BY id DESC
      LIMIT 1
      |sql}
  )];

let wrap_lwt_result = (result_lwt, _error_message) => {
  let%lwt result = result_lwt;
  switch result {
  | Ok () => () |> Lwt.return
  | Error(err) => failwith((Caqti_error.show(err)));
  }
}

let get_current_index = (migrations) => {
  open Lwt_result;
  let pool = make_pool();
  let execute = (query) => Caqti_lwt.Pool.use(query, pool);
  execute(get_last_migration()) >|= fun (current_opt) => {
    switch current_opt {
    | Some((filename, is_up, _applied)) => {
        let cp = fun (_i, name) => String.compare(name, filename) == 0;
        let index = fst(Option.value_exn(List.findi(migrations, ~f=cp)));
        if (is_up) {
          index + 1
        } else {
          index
        }
      }
    | None => 0
    }
  };
}

let log_migration = (~filename: string, ~up: bool, ~applied) => {
  let f = filename;
  [%rapper
    execute(
      {sql|
      INSERT INTO schema_migrations (migration, up, applied)
      VALUES (%string{migration}, %bool{up}, %ptime{applied})
      |sql})](~migration=f, ~up, ~applied)
}

let run = (execute, start, migrations, target) => {
  let (move, dir, going_up) =
    if (start < target) { ((fun (x) => x + 1), up_dir, true) }
    else { ((fun (x) => x - 1), down_dir, false) };

  let rec iter = (current) => {
    if (current == target) { Lwt.return_unit }
    else {
      let filename = if (going_up) {
        List.nth_exn(migrations, current)
      } else {
        List.nth_exn(migrations, current - 1)
      };

      let migration = dir / filename;
      Stdio.print_endline(migration);

      let cmd = Printf.sprintf("psql -U %s -d %s -h %s -f %s", "postgres", "todo", "127.0.0.1", migration);
      let res = Core_unix.system(cmd);

      switch res {
      | Ok (()) => {
          let%lwt log_result = {
            let applied = Ptime_clock.now(());

            if (going_up) {
              execute(log_migration(~filename, ~up=true, ~applied))
            } else {
              execute(log_migration(~filename, ~up=false, ~applied))
            }
          }

          switch log_result {
          | Ok () => iter(move(current))
          | Error(_) => failwith("Error logging migration to DB")
          }
        }
      | Error(_) => Stdio.prerr_endline("Error running migration") |> Lwt.return;
      }
    }
  }

  let%lwt () = iter(start);
  Ok(()) |> Lwt.return
}

let do_migration = (migrations) => {
  let target = List.length(migrations);
  let pool = make_pool();
  let execute = (query) => Caqti_lwt.Pool.use(query, pool);
  let main = () => {
    let run_result = {
      open Lwt_result;
      get_current_index(migrations) >>= fun (start) => run(execute, start, migrations, target);
    }
    wrap_lwt_result(run_result, "Error getting migration history from DB");
  };

  Lwt_main.run(main())
}

let migrate () = {
  print_endline("Migrating...");
  do_migration(get_migrations());
}

let revolt_t = {
  open Term;
  const(migrate) $ const(());
};

let cmd = Cmd.v(Cmd.info("migrate"))(revolt_t);

let () = exit (Cmd.eval(cmd));
