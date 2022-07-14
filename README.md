# Web-based TODO app in Reason + [Melange](https://github.com/melange-re/melange)

The motivation of this project is to learn Reason + Melange ecosystem, basically Reason-based web development.
There will be lots of extra/unnecessary shit in this repo due to the nature of this project being just a Reason experimentation.

For example, CSS styles are implemented in multiple different ways in order to learn both ReasonReact inline styles
and very cool [styled-ppx](https://github.com/davesnx/styled-ppx).
Similarly, backend is kept very basic. Lots of stuff has been implemented manually for learning purposes.
The architecture of the backend is hugely inspired by [Ocoi](https://github.com/roddyyaga/ocoi)

## Project structure
`client` - Melange web app with Rescript React
`server` - Native Reason backend
`common` - Shared logic between client and server, i.e DTO classes

## Build
Frontend
```bash
npm install
esy mel build
```

Backend
```bash
esy build
```

## How to run

Launch Postgres docker container
```bash
docker compose up
```

Launch Reason backend
```bash
esy server:start
```

Frontend

```bash
npm run serve
```

Finally, navigate to http://localhost:8080
