module type None = {
  type t = unit;
}

module None = {
  type t = unit;
}

module Make = {
  module None (Parameters : None) = {
    let f = (_req) => () |> Lwt_result.return;
  }
}
