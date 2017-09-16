type stream 'a;

module Unfold = {
  type t 'value 'seed =
    | Value 'value 'seed
    | Done;
};

external convertUnfoldValue : Js.t {. _done : bool} => Js.t {. seed : 'a, value : 'b} =
  "%identity";

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

external _unfold : ('a => 'whatever) => 'a => stream 'b = "unfold" [@@bs.module "most"];

/* Creates a stream from a generating function and a seed */
let unfold f seed =>
  _unfold
    (
      fun x =>
        switch (f x) {
        | Unfold.Done => convertUnfoldValue {"_done": true}
        | Unfold.Value value seed => {"value": value, "seed": seed}
        }
    )
    seed;

/* Creates a stream from a list */
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

external fromPromise : Js.Promise.t 'a => stream 'a = "" [@@bs.module "most"];

external periodic : int => stream unit = "" [@@bs.module "most"];

external never : unit => stream 'a = "" [@@bs.module "most"];

external iterate : ('a => 'a) => 'a => stream 'a = "" [@@bs.module "most"];

external iteratePromise : ('a => Js.Promise.t 'a) => 'a => stream 'a =
  "iterate" [@@bs.module "most"];

/*external generate*/
external fromEvent : string => Dom.eventTarget => Js.boolean => stream Dom.event =
  "" [@@bs.module "most"];

/*external fromEventEmitter*/
/* Concatenates two streams together */
external concat : stream 'a => stream 'a = "" [@@bs.send.pipe : stream 'a];

/* Appends an element to the start of a stream */
let startWith x obs => concat obs (just x);


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

external timestamp : stream 'a => stream (Js.t {. time: int, value: 'a }) = "" [@@bs.module "most"];

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
