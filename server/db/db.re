open Core;

let hostname = Sys.getenv("POSTGRES_HOSTNAME") |> fun
  | Some(s) => s
  | None => "localhost";

let hostname_to_connection_uri = hostname =>
  Printf.sprintf("postgresql://postgres:postgres@%s:5432/todo", hostname);

let db_url = hostname_to_connection_uri(hostname);

let connect = connection_uri => {
  connection_uri |> Uri.of_string |> Caqti_lwt.connect_pool(~max_size=10)
  |> fun
  | Ok(pool) => pool
  | Error(err) => failwith((Caqti_error.show(err)));
}

let query_pool = (query, pool) =>
  Caqti_lwt.Pool.use(query, pool);

let pool = connect(db_url);

let run_query = query => query_pool(query, pool);
