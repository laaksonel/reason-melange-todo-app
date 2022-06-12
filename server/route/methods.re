open Opium;

type verb = Get | Post | Put | Delete

let verb_to_string = v => switch v {
  | Get => "get"
  | Post => "post"
  | Put => "put"
  | Delete => "delete"
}

let verb_to_route = verb => {
  switch verb {
  | Get => App.get
  | Post => App.post
  | Put => App.put
  | Delete => App.delete
  }
}
