open Utils;

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
