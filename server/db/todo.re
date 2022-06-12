[@deriving yojson]
type t = {
  id: int,
  title: string,
  completed: bool
};

module No_id = {
  [@deriving yojson]
  type t = {
    title: string,
    completed: bool
  };
}

