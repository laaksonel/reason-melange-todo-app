type state = {items: list(TodoItem.t)};

let emptyState: state = {items: []};

type actions =
  | AddItem(TodoItem.t)
  | DeleteItem(int)
  | ToggleItem(int)
  | SetItems(option(TodoItemCollection.t));

let add = (newItem: TodoItem.t, state: state) => {
  {items: [newItem, ...state.items]};
};

let delete = (id, state) => {
  items: state.items |> List.filter((item: TodoItem.t) => item.id != id),
};

let toggle = (id, state) => {
  items:
    state.items
    |> List.map((item: TodoItem.t) => {
         item.id == id ? {...item, completed: !item.completed} : item
       }),
};

let set = (items: option(TodoItemCollection.t), _state: state) => {
  switch (items) {
  | Some(items) => {items: items.items}
  | None => emptyState
  };
};

let reducer = (state, action) => {
  switch (action) {
  | AddItem(newItem) => add(newItem, state)
  | DeleteItem(id) => delete(id, state)
  | ToggleItem(id) => toggle(id, state)
  | SetItems(items) => set(items, state)
  };
};

let store = Restorative.createStore(emptyState, reducer);
