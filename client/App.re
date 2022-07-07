module TodoList = [%styled.div
  {|
    display: flex;
    justify-content: center;
    flex-direction: column;
    align-items: center;
  |}
];

module Topic = [%styled.div
  {|
    font-size: 2rem;
    display: flex;
    justify-content: center;
    color: white;
    font-family: "Roboto";
    padding: 20px 0 20px 0;
  |}
];

module AddCircle = {
  [@bs.module "@material-ui/icons/AddCircle"] [@react.component]
  external make:
    (
      ~color: string=?,
      ~className: string=?,
      ~fontSize: string=?,
      ~htmlColor: string=?
    ) =>
    React.element =
    "default";
};

[@react.component]
let make = () => {
  let state = TodoStore.store.useStore();

  React.useEffect0(() => {
    TodoService.fetchTodos();
    None;
  });

  let (show, setShow) = React.useState(_ => false);

  let openModal = _ => {
    setShow(_ => true);
  };

  let closeModal = _ => {
    setShow(_ => false);
  };

  <>
    <Topic> {React.string("TODO")} </Topic>
    <Mui.IconButton onClick=openModal>
      <AddCircle htmlColor="#f50057" />
    </Mui.IconButton>
    <TodoList>
      {state.items
       |> List.map((item: TodoItem.t) =>
            <TodoListItem key={string_of_int(item.id)} item />
          )
       |> Array.of_list
       |> React.array}
    </TodoList>
    <AddTodoModal
      show
      onCancel=closeModal
      onSubmit={text =>
                  TodoService.createTodo({
                    id: 0,
                    title: text,
                    completed: false,
                  }) |> closeModal}
    />
  </>;
};
