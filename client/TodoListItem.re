[@react.component]
let make = (~id: int) => {
  <li>
  { React.int(id) }
  </li>
};
