open Utils;

module type Json = {
  type t

  let yojson_of_t : t => Yojson.Safe.t
}

module Json_list (Json : Json) = Json

module type Json_opt = Json

module Json_opt (Json : Json) = Json

module type No_content = {
  type t = unit
}

module No_content = Unit

module Created = {
  module type Int = {
    type t = int
  }

  module Int = Int
}

module type String = {
  type t = string;
}

module String = {
  type t = string;
}

module Make = {
  module String (Responses : String) = {
    let f = (s) => `String(s) |> respond
  }
}
