# bs-most

BuckleScript bindings for Most.js

```reason
let add x y => x + y;
let isEven x => x mod 2 === 0;

/**
 * Logs:
 * 2
 * 4
 * 6
 * 8
 * 10
 * Stream completed
 **/
Most.(
  fromList [0,1,2,3,4,5,6,7,8]
  |> map (add 2)
  |> filter isEven
  |> observe (fun x => Js.log x)
  |> Js.Promise.then_ (fun _ => {
    Js.log "Stream completed";
    Js.Promise.resolve ()
  })
);
```

## What's missing

1. `generate` - Reason doesn't have generator support yet
2. `fromEvent` - DOM events are supported, but generic `EventEmitter`'s are not yet
3. `transduce` - Awkward to use in Reason/OCaml without a solid transducer impl (maybe coming soon?? 😉)

# Build
```
npm run build
```

# Build + Watch

```
npm run watch
```

# Testing

```
npm run test
```
