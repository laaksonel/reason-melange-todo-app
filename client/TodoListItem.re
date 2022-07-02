[@react.component]
let make = (~item: TodoItem.t) => {
  <li>
  { React.int(item.id) }
  { React.string(item.title) }
  { item.completed |> string_of_bool |> React.string }
  </li>
};
