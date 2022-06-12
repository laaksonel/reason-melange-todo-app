open Domain;
// TODO: Create ppx for IDless versions
let create = ({ title, _ }: Todo.t) => {
  Logs.info(m => m("Called %s", title));
}
