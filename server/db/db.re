open Core;
open Opium;

let hostname = Sys.getenv("POSTGRES_HOSTNAME") |> fun
  | Some(s) => s
  | None => "localhost";

let hostname_to_connection_uri = hostname =>
  Printf.sprintf("postgresql://todo@%s:5432/todo", hostname);

let connect = connection_uri =>
  connection_uri |> Uri.of_string |> Caqti_lwt.connect_pool(~max_size=10)
  |> fun
  | Ok(pool) => pool
  | Error(err) => failwith((Caqti_error.show(err)));

let query_pool = (query, pool) =>
  Caqti_lwt.Pool.use(query, pool) |> Lwt_result.map_err(Caqti_error.show);

type caqti_conn_pool('err) =
  Caqti_lwt.Pool.t(Caqti_lwt.connection, 'err);

type db_pool('err) = caqti_conn_pool('err);

let key: Opium.Context.key(db_pool(Caqti_error.connect)) =
  Opium.Context.Key.create(("db pool", (_x) => sexp_of_string("db_pool")));

let middleware = app => {
  let db_url = hostname_to_connection_uri(hostname);
  let pool = connect(db_url);

  let filter = handler => (req: Request.t) => {
    let env = Opium.Context.add(key, pool, req.Request.env);
    handler({ ...req, env })
  }

  let m = Rock.Middleware.create(~name="database connection pool", ~filter);
  App.middleware(m, app);
}

let query_db = (query, req) => req.Request.env
  |> Opium.Context.find_exn(key)
  |> query_pool(query);
