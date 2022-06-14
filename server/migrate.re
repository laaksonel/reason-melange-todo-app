open Cmdliner;

let revolt () = print_endline("Revolt!");

let revolt_t = {
  open Term;
  const(revolt) $ const(());
}

let cmd = Cmd.v(Cmd.info("revolt"))(revolt_t)

let () = exit (Cmd.eval(cmd));
