type customer = {
  customerId: int,
  name: string,
  lastPayDate: option(string),
  balance: float
};

type request =
| GetCustomer(int)
| PayBill({. "customerId": int, "payDate": string, "amount": float});

type response = Customer(customer) | BillRef(int);
