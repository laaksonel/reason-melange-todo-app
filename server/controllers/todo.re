open Domain;
// TODO: Create ppx for IDless versions
let create = ({ title, completed, _ }: Todo.No_id.t) => {
  Logs.info(m => m("Creating a new todo (%s, %s)", title, string_of_bool(completed)));

  let create =
    [%rapper
      get_one(
        {sql|
        INSERT INTO todos (title, completed)
        VALUES (%string{title}, %bool{completed})
        RETURNING @int{id}
        |sql})]

  Db.run_query(create(~title, ~completed=false));
}

let index = () => [] |> Lwt_result.return
