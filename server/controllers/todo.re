open Domain.Todo;

let get_todos = (q, ()) => {
  let todos =
    [%rapper
      get_many(
        {sql|
        SELECT @int{id}, @string{title}, @bool{completed}
        FROM todos
        |sql},
        record_out,
      )
    ]();

  q(todos);
};

let show = (q, id) => {
  let get_todo = [%rapper
    get_opt(
      {sql|
        SELECT @int{id}, @string{title}, @bool{completed}
        FROM todos
        WHERE id = %int{id}
        |sql},
      record_out,
    )
  ];

  q(get_todo(~id));
};

// TODO: Create ppx for IDless versions
let create = (q, {title, completed}: No_id.t) => {
  Logs.info(m =>
    m("Creating a new todo (%s, %s)", title, string_of_bool(completed))
  );

  let create = [%rapper
    get_one(
      {sql|
        INSERT INTO todos (title, completed)
        VALUES (%string{title}, %bool{completed})
        RETURNING @int{id}
        |sql},
      function_out,
    )
  ];

  let todo_model = (~id) => {id, title, completed};

  q(create(~title, ~completed, todo_model));
};

let update = (q, todo) => {
  let update = [%rapper
    execute(
      {sql|
        UPDATE todos
        SET id = %int{id}, title = %string{title}, completed = %bool{completed}
        WHERE id = %int{id}
        |sql},
      record_in,
    )
  ];

  q(update(todo));
};
