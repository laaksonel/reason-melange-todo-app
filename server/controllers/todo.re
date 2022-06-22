open Domain.Todo;


let get_todos = (q) => () => {
  let todos =
    [%rapper
      get_many(
        {sql|
        SELECT @int{id}, @string{title}, @bool{completed}
        FROM todos
        |sql}, record_out)
    ]();

  q(todos);
}

// TODO: Create ppx for IDless versions
let create = (q) => ({ title, completed, _ }: No_id.t) => {
  Logs.info(m => m("Creating a new todo (%s, %s)", title, string_of_bool(completed)));

  let create =
    [%rapper
      get_one(
        {sql|
        INSERT INTO todos (title, completed)
        VALUES (%string{title}, %bool{completed})
        RETURNING @int{id}
        |sql})]

  q(create(~title, ~completed=false));
}

