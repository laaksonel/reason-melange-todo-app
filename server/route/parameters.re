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

module Path = {
  module type One = {
    type t
    let of_string : string => t
  }

  module One = {
    module Int = {
      type t = int
    }
  }
}
