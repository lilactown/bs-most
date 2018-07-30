# bs-most

BuckleScript bindings for [Most.js](https://github.com/cujojs/most).

In [Reason](https://reasonml.github.io/):
```reason
let add = (x, y) => x + y;

let isEven = x => x mod 2 === 0;

Most.(
  fromList([0, 1, 2, 3, 4, 5, 6, 7, 8])
  |> map(add(2))
  |> filter(isEven)
  |> observe(x => Js.log(x))
  |> Js.Promise.then_((_) => {
       Js.log("Stream completed");
       Js.Promise.resolve();
     })
);


/**
 * Logs:
 * 2
 * 4
 * 6
 * 8
 * 10
 * Stream completed
 **/
```

An example of implementing the Redux architecture in a ReasonReact project can be [seen here](https://github.com/Lokeh/reason-autocomplete).

## How do I install it?

Inside of a BuckleScript project:

```
npm install --save bs-most
```

Then add `bs-most` to your `bs-dependencies` in `bsconfig.json`:

```
{
  ...
  "bs-dependencies": [
    "bs-most"
  ]
}
```

## How do I use it?

The Most.js [API docs](https://github.com/cujojs/most/blob/master/docs/api.md) are probably the best source of information about each method. This library attempts to provide very low-level bindings, except for what's noted in the **What's different** section.

The bindings are a single file in `src/most.re`. I try to provide comments next to each binding and am working on flushing out tests so that users can refer to them.

I am also available by e-mail or DM on the Reason discord (username: @lilactown). 

## What's missing

1. `generate` - Reason doesn't have generator support yet
2. `transduce` - Awkward to use in Reason/OCaml without a solid transducer impl (maybe coming soon?? 😉)
3. `combineArray` - the callback for this is variadic (depending on how many streams you have in the array), not sure how to type it yet 😞

## What's different

1. Predicates that normally return "truthy" values, now only accept returning booleans
2. `sample` is variadic so we offer `sample1`, `sample2`, `sample3`... `sample6`
3. `fromEvent` - DOM events are supported, but generic `EventEmitter`'s are not yet
4. `from` only works on arrays right now; generic iterators must be coerced
5. `fromList` is a function that creates a stream from a Reason `List` type, e.g. `fromList([1, 2, 3])`
6. `unfold` is modeled a bit differently - instead of taking in a JS object `{ done, value, seed }`, it takes an `option (value, seed)`.
Example:
```reason
/* emits 2,4,6 then completes */
Most.unfold(
  (seed) =>
    if (seed < 4) {
      let nextSeed = seed + 1;
      Some((seed * 2, nextSeed));
    } else {
      None;
    },
  1
);
```

# Subjects

This library also includes some bindings for [Subjects](https://github.com/mostjs-community/subject). Subjects are very useful for e.g. implementing a state store/dispatch system. The API is a little bit different from the raw `most-subject` library:

- `Subject.make`: creates an [asynchronous Subject](https://github.com/mostjs-community/subject#asynct-subjectt)
- `Subject.asStream`: casts a subject to a stream, to be used by the rest of the `Most` module
- `Subject.next`: emits a value on a subject
- `Subject.error`: emits an error on a subject
- `Subject.complete`: completes a subject with no value
- `Subject.completeWith`: completes a subject with a value

## Build
```
npm run build
```

## Build + Watch

```
npm run watch
```

## Testing

```
npm run test
```
