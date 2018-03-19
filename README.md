# Resilient NodeJS services with ReasonML and Nact

Get current balance:

curl -X GET localhost:3000/customer/1234

Pay bill:

curl -H "Content-Type: application/json" -d '{"customerId": 1234, "payDate": "2018-03-19", "amount": 30}' localhost:3000/bill/pay
