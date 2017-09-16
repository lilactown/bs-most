type stream 'a; /* = Js.t {.
  observe [@bs.meth] : ('a => unit) => Js.Promise.t unit,
  concat [@bs.meth]: stream 'a => stream 'a,
  map [@bs.meth]: ('a => )
};*/

/* observes a stream */
external observe : ('a => unit) => Js.Promise.t unit = "" [@@bs.send.pipe : stream 'a];

/* Creates a stream containing only x */
external just : 'a => stream 'a = "" [@@bs.module "most"];

/* Creates a stream from an iterable or observable */
external from : array 'a => stream 'a = "" [@@bs.module "most"];

/* Creates an already ended stream with no events */
external empty : unit => stream unit = "" [@@bs.module "most"];

external concat : stream 'a => stream 'a = "" [@@bs.send.pipe : stream 'a];
let startWith x obs => concat obs (just x);
