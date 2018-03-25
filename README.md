# Resilient NodeJS services with ReasonML and Nact

This is the project demoed at the TorontoJS Meetup:
https://www.meetup.com/torontojs/events/248447403/

Slides are available at: https://docs.google.com/presentation/d/1KS2cZUP1pSBQlxnr4vdprrBe1iAdzzW79F_9O448ObM/edit?usp=sharing

This project shows an example of using a single Node/Express app to
handle, in a fault-tolerant way, two different endpoints that in
traditional microservice architecture might be split up into two
services.

## Build

You'll need to install BuckleScript:

    npm install -g bs-platform

Then, locally install this project's dependencies:

    cd resilient-services
    npm install

Then, build:

    bsb

If you want to hack on it, you can run the BuckleScript build tool in
watch mode:

    bsb -w

## Try

Get current balance:

    curl -X GET localhost:3000/customer/1234

Pay bill:

    curl -H "Content-Type: application/json" -d '{"customerId": 1234, "payDate": "2018-03-19", "amount": 30}' localhost:3000/bill/pay 
