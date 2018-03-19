/** The main business logic of our system. Contains an immutable store of
    customers. Simplified model of a persistent store. */

open Types;
module IntMap = Map.Make({type t = int; let compare = compare;});

/* Some dummy data to start with. */
let initial = IntMap.empty
  |> IntMap.add(
    1234,
    { customerId: 1234,
      name: "Bob Roberts",
      lastPayDate: None, balance: 150. });

let payBill(bill, store) = {
  let customerId = bill##customerId;
  let customer = IntMap.find(customerId, store);
  let customer = {
    ...customer,
    lastPayDate: Some(bill##payDate),
    balance: customer.balance -. bill##amount
  };

  (IntMap.add(customerId, customer, store), Hashtbl.hash(customer))
};

let info = IntMap.find;

let encode({customerId, name, lastPayDate, balance}) = {
  open! Json.Encode;

  object_([
    ("customerId", int(customerId)),
    ("name", string(name)),
    ("lastPayDate", nullable(string, lastPayDate)),
    ("balance", float(balance))
  ])
};
