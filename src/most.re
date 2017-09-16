type stream 'a;

/*type unfoldValue 'value 'seed =
  | Value 'value 'seed
  | Done;*/
module Unfold = {
  type t 'value 'seed =
    | Value 'value 'seed
    | Done;
};

external convertUnfoldValue : Js.t {. _done : bool} => Js.t {. seed : 'a, value : 'b} =
  "%identity";

/*unfoldValue_make a => switch
 */
/* observes a stream */
external observe : ('a => unit) => Js.Promise.t unit = "" [@@bs.send.pipe : stream 'a];

/* Creates an already ended stream with no events */
external empty : unit => stream unit = "" [@@bs.module "most"];

/* Creates a stream containing only x */
external just : 'a => stream 'a = "" [@@bs.module "most"];

/* Creates a stream from an array */
external from : array 'a => stream 'a = "" [@@bs.module "most"];

external _unfold : ('a => 'whatever) => 'a => stream 'b = "unfold" [@@bs.module "most"];

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

external concat : stream 'a => stream 'a = "" [@@bs.send.pipe : stream 'a];

let startWith x obs => concat obs (just x);
