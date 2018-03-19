/** This service acts as a sanity check for bill payment requests that
    are coming in to the server. If any bill payments come through that
    don't meet our criteria, let it crash and restart. */

open Types;

let decode(json) = {
  open! Json.Decode;

  { "customerId": field("customerId", int, json),
    "payDate": field("payDate", string, json),
    "amount": field("amount", float, json) }
};

let make(parent, customerService) = Nact.spawnStateless(
  ~name=__MODULE__,
  ~onCrash=(_, _, _) => Js.Promise.resolve(Nact.Reset),
  parent,
  ((caller, bill), _ctx) => {
    assert(bill##amount >= 25.);
    assert(bill##customerId >= 1);
    assert(bill##payDate != "");

    Nact.query(
      ~timeout=100 * Nact.milliseconds,
      customerService,
      caller' => (caller', PayBill(bill)))
    |> Js.Promise.then_(fun
      | BillRef(id) => id |> Nact.dispatch(caller) |> Js.Promise.resolve
      | _ => Js.Promise.resolve())
  });
