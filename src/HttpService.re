/** The HTTP server and its associated actor. */

open Types;

let timeout = 100 * Nact.milliseconds;
let port = 3_000;

let express(billService, customerService) = {
  open Express;

  let app = express();

  App.use(app, Middleware.json());

  App.get(
    app,
    ~path="/customer/:id",
    PromiseMiddleware.from((_, req, res) =>
      Nact.query(
        ~timeout,
        customerService,
        caller =>
          ( caller,
            GetCustomer(
              req
              |> Request.params
              |> Js.Dict.unsafeGet(_, "id")
              |> Json.Decode.string
              |> int_of_string) ))
      |> Js.Promise.then_(fun
        | Customer(customer) => res
          |> Response.sendJson(Store.encode(customer))
          |> Js.Promise.resolve
        | _ => failwith("Shouldn't happen"))));

  App.post(app, ~path="/bill/pay", PromiseMiddleware.from((_, req, res) =>
    Nact.query(~timeout, billService, caller =>
      ( caller,
        req |> Request.bodyJSON |> Js.Option.getExn |> BillService.decode ))
    |> Js.Promise.then_(billId => res
      |> Response.sendJson(Json.Encode.int(billId))
      |> Js.Promise.resolve)));

  App.listen(
    app,
    ~port,
    ~onListen=err => Js.log(
      if (Js.Nullable.test(err)) {j|Started HTTP server on port $port|j}
      else "Could not start HTTP server"),
    ());

  app
};

let make(parent, billService, customerService) = Nact.spawn(
  ~name=__MODULE__,
  parent,
  (express, (), _) => Js.Promise.resolve(express),
  express(billService, customerService));
