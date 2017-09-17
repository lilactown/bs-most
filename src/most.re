type stream 'a;

/* observes a stream */
external observe : ('a => unit) => Js.Promise.t unit = "" [@@bs.send.pipe : stream 'a];

external forEach : ('a => unit) => Js.Promise.t unit = "" [@@bs.send.pipe : stream 'a];


/**
 * Stream creation
 **/
/* Creates an already ended stream with no events */
external empty : unit => stream unit = "" [@@bs.module "most"];

/* Creates a stream containing only x */
external just : 'a => stream 'a = "" [@@bs.module "most"];

/* Creates a stream from an array */
external from : array 'a => stream 'a = "" [@@bs.module "most"];

/* `unfold` is hard to type.
 * It takes an object of the shape { seed, value, done }
 * where done is a boolean whether to complete the stream.
 * When `done` is true, `seed` and `value` are ignored.
 * There are often times when we may complete a stream after
 * exhausting our data source (e.g. the `fromList` function),
 * so we need a way of saying we're done without providing
 * a `value` or a `seed`.
 * We expose an unsafe `_unfold` here, and then below a more
 * typesafe version along with some types to go along with it.
 */
external _unfold : ('a => Js.t 'whatever) => 'a => stream 'b = "unfold" [@@bs.module "most"];

module Unfold = {
  type t 'value 'seed =
    | Value 'value 'seed
    | Done;
};

external cast : Js.t 'a => Js.t 'b = "%identity";

/* Creates a stream from a generating function and a seed */
let unfold f =>
  _unfold
    (
      fun x =>
        switch (f x) {
        | Unfold.Done => cast {"_done": true}
        | Unfold.Value value seed => cast {"value": value, "seed": seed}
        }
    );

/* Creates a stream from a Reason list */
let fromList list =>
  unfold
    (
      fun curList =>
        switch curList {
        | [] => Unfold.Done
        | [x] => Unfold.Value x []
        | [x, ...rest] => Unfold.Value x rest
        }
    )
    list;

/* Creates a stream from a promise that completes once the promise resolves */
external fromPromise : Js.Promise.t 'a => stream 'a = "" [@@bs.module "most"];

/* Create an infinite stream containing events that arrive every period milliseconds, and whose value is undefined. */
external periodic : int => stream unit = "" [@@bs.module "most"];

/* Create a stream that contains no events and never ends */
external never : unit => stream 'a = "" [@@bs.module "most"];

/* Build an infinite stream by computing successive items iteratively. Conceptually, the stream will contain: [initial, f(initial), f(f(initial)), ...] */
external iterate : ('a => 'a) => 'a => stream 'a = "" [@@bs.module "most"];

/* Same as `iterate`, but the function may return a promise. This allows one to build asynchronous streams of future values */
external iteratePromise : ('a => Js.Promise.t 'a) => 'a => stream 'a =
  "iterate" [@@bs.module "most"];

/* Create a stream of events from a DOM EventTarget */
external fromEvent : string => Dom.eventTarget => Js.boolean => stream Dom.event =
  "" [@@bs.module "most"];

/* Concatenates two streams together */
external concat : stream 'a => stream 'a = "" [@@bs.send.pipe : stream 'a];

/* Appends an element to the start of a stream */
external startWith : 'a => stream 'a => stream 'a = "" [@@bs.module "most"];


/**
 * Error handling
 **/
external recoverWith : (Js.Exn.t => stream 'a) => stream 'a => stream 'a = "" [@@bs.module "most"];

external throwError : Js.Exn.t => stream unit = "" [@@bs.module "most"];


/**
 * Transforming
 **/
external map : ('a => 'b) => stream 'a => stream 'b = "" [@@bs.module "most"];

external constant : 'a => stream 'b => stream 'a = "" [@@bs.module "most"];

external scan : ('accum => 'a => 'b) => 'a => stream 'a => stream 'b = "" [@@bs.module "most"];

external flatMap : ('a => stream 'b) => stream 'a => stream 'b = "" [@@bs.module "most"];

external continueWith : ('a => stream 'b) => stream 'a => stream 'b = "" [@@bs.module "most"];

external concatMap : ('a => stream 'b) => stream 'a => stream 'b = "" [@@bs.module "most"];

external ap : stream ('a => 'b) => stream 'a => stream 'b = "" [@@bs.module "most"];

external timestamp : stream 'a => stream (Js.t {. time : int, value : 'a}) =
  "" [@@bs.module "most"];

external tap : ('a => unit) => stream 'a => stream 'a = "" [@@bs.module "most"];


/**
 * Filtering
 **/
external filter : ('a => bool) => stream 'a => stream 'a = "" [@@bs.module "most"];


/**
 * Slicing
 **/
external slice : int => int => stream 'a => stream 'a = "" [@@bs.module "most"];

external take : int => stream 'a => stream 'a = "" [@@bs.module "most"];

external skip : int => stream 'a => stream 'a = "" [@@bs.module "most"];


/**
 * Combining
 **/
external merge : stream 'a => stream 'a => stream 'a = "" [@@bs.module "most"];

external combine : ('a => 'b => 'c) => stream 'a => stream 'b => stream 'c =
  "" [@@bs.module "most"];

external zip : ('a => 'b => 'c) => stream 'a => stream 'b => stream 'c = "" [@@bs.module "most"];
