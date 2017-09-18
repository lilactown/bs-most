type stream 'a;

/* observes a stream */
external observe : ('a => unit) => stream 'a => Js.Promise.t unit = "" [@@bs.module "most"];

external forEach : ('a => unit) => stream 'a => Js.Promise.t unit = "" [@@bs.module "most"];

/* Reduce a stream, returning a promise for the ultimate result. */
external reduce : ('accum => 'a => 'b) => 'b => stream 'a => Js.Promise.t 'b =
  "" [@@bs.module "most"];

/* Start consuming events from stream.
   This can be useful in some cases where you don't want or need to process the terminal events
   --e.g. when all processing has been done via upstream side-effects.
   Most times, however, you'll use observe to consume and process terminal events. */
external drain : stream 'a => Js.Promise.t unit = "" [@@bs.module "most"];

/* Draft ES Observable compatible subscribe. Start consuming events from stream by providing an observer object. */
type observer 'a = Js.t {. next : 'a => unit, error : Js.Exn.t => unit, complete : unit => unit};

type subscription = Js.t {. unsubscribe : unit => unit};

external subscribe : observer 'a => subscription = "" [@@bs.send.pipe : stream 'a];


/**
 * Stream creation
 **/
/* Creates an already ended stream with no events */
external empty : unit => stream unit = "" [@@bs.module "most"];

/* Creates a stream containing only x */
external just : 'a => stream 'a = "" [@@bs.module "most"];

/* Creates a stream from an array */
external from : array 'a => stream 'a = "" [@@bs.module "most"];

/* The JavaScript version of `unfold` is hard to type safely.
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

external unsafeCast : Js.t 'a => Js.t 'b = "%identity";

/* Creates a stream from a generating function and a seed */
let unfold f =>
  _unfold (
    fun x =>
      switch (f x) {
      | None => unsafeCast {"_done": true}
      | Some (value, seed) => unsafeCast {"value": value, "seed": seed}
      }
  );

/* Creates a stream from a Reason list */
let fromList list =>
  unfold
    (
      fun curList =>
        switch curList {
        | [] => None
        | [x, ...rest] => Some (x, rest)
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
/* Recover from a stream failure by calling a function to create a new stream. */
external recoverWith : (Js.Exn.t => stream 'a) => stream 'a => stream 'a = "" [@@bs.module "most"];

/* Create a stream in the error state. */
external throwError : Js.Exn.t => stream unit = "" [@@bs.module "most"];


/**
 * Transforming
 **/
/* Create a new stream by applying f to each event of the input stream. */
external map : ('a => 'b) => stream 'a => stream 'b = "" [@@bs.module "most"];

/* Create a new stream by replacing each event of the input stream with x */
external constant : 'a => stream 'b => stream 'a = "" [@@bs.module "most"];

/* Create a new stream containing incrementally accumulated results, starting with the provided initial value. */
external scan : ('accum => 'a => 'b) => 'a => stream 'a => stream 'b = "" [@@bs.module "most"];

/* Transform each event in stream into a stream, and then merge it into the resulting stream. */
external flatMap : ('a => stream 'b) => stream 'a => stream 'b = "" [@@bs.module "most"];

/* Replace the end signal with a new stream returned by f. */
external continueWith : ('a => stream 'b) => stream 'a => stream 'b = "" [@@bs.module "most"];

/* Transform each event in stream into a stream, and then concatenate it onto the end of the resulting stream. */
external concatMap : ('a => stream 'b) => stream 'a => stream 'b = "" [@@bs.module "most"];

/* Apply the latest function in a stream of functions to the latest value in stream */
external ap : stream ('a => 'b) => stream 'a => stream 'b = "" [@@bs.module "most"];

/* Materialize event timestamps, transforming Stream<X> into Stream<{ time:number, value:X }> */
external timestamp : stream 'a => stream (Js.t {. time : int, value : 'a}) =
  "" [@@bs.module "most"];

/* Perform a side-effect for each event in stream. */
external tap : ('a => unit) => stream 'a => stream 'a = "" [@@bs.module "most"];


/**
 * Filtering
 **/
/* Create a stream containing only events for which the predicate returns true. */
external filter : ('a => Js.boolean) => stream 'a => stream 'a = "" [@@bs.module "most"];

/* Create a new stream with adjacent repeated events removed. */
external skipRepeats : stream 'a => stream 'a = "" [@@bs.module "most"];

/* Create a new stream with adjacent repeated events removed, using the provided comparison function */
external skipRepeatsWith : ('a => 'a => Js.boolean) => stream 'a => stream 'a =
  "" [@@bs.module "most"];


/**
 * Slicing
 **/
/* Create a new stream containing only events where start <= index < end, where index is the ordinal index of an event in stream.
   If stream contains fewer than start events, the returned stream will be empty. */
external slice : int => int => stream 'a => stream 'a = "" [@@bs.module "most"];

/* Create a new stream containing at most n events from stream.
   If stream contains fewer than n events, the returned stream will be effectively equivalent to stream. */
external take : int => stream 'a => stream 'a = "" [@@bs.module "most"];

/* Create a new stream that omits the first n events from stream.
   If stream contains fewer than n events, the returned stream will be empty.  */
external skip : int => stream 'a => stream 'a = "" [@@bs.module "most"];

/* Create a new stream containing all events until predicate returns false. */
external takeWhile : ('a => Js.boolean) => stream 'a => stream 'a = "" [@@bs.module "most"];

/* Create a new stream containing all events after predicate returns false. */
external skipWhile : ('a => Js.boolean) => stream 'a => stream 'a = "" [@@bs.module "most"];

/* Create a new stream containing all events before and including when the predicate returns true. */
external skipAfter : ('a => Js.boolean) => stream 'a => stream 'a = "" [@@bs.module "most"];

/* Create a new stream containing all events until endSignal emits an event. */
external until : stream 'b => stream 'a => stream 'a = "" [@@bs.module "most"];

/* Create a new stream containing all events after startSignal emits its first event. */
external since : stream 'b => stream 'a => stream 'a = "" [@@bs.module "most"];

/* Create a new stream containing only events that occur during a dynamic time window. */
external during : stream (stream 'ending) => stream 'a => stream 'a = "" [@@bs.module "most"];


/**
 * Combining
 **/
/* Create a new stream containing events from stream1 and stream2. */
external merge : stream 'a => stream 'a => stream 'a = "" [@@bs.module "most"];

/* Create a new stream that emits the set of latest event values from all input streams
   whenever a new event arrives on any input stream. */
external combine : ('a => 'b => 'c) => stream 'a => stream 'b => stream 'c =
  "" [@@bs.module "most"];

/* Create a new stream by combining sampled values from many input streams. */
external sample2 : ('a => 'a => 'b) => stream 'a => stream 'a => stream 'b =
  "sample" [@@bs.module "most"];

external sample3 : ('a => 'a => 'a => 'b) => stream 'a => stream 'a => stream 'a => stream 'b =
  "sample" [@@bs.module "most"];

external sample4 :
  ('a => 'a => 'a => 'a => 'b) => stream 'a => stream 'a => stream 'a => stream 'a => stream 'b =
  "sample" [@@bs.module "most"];

external sample5 :
  ('a => 'a => 'a => 'a => 'a => 'b) =>
  stream 'a =>
  stream 'a =>
  stream 'a =>
  stream 'a =>
  stream 'a =>
  stream 'b =
  "sample" [@@bs.module "most"];

external sample6 :
  ('a => 'a => 'a => 'a => 'a => 'a => 'b) =>
  stream 'a =>
  stream 'a =>
  stream 'a =>
  stream 'a =>
  stream 'a =>
  stream 'a =>
  stream 'b =
  "sample" [@@bs.module "most"];

/* When an event arrives on asampler, emit the latest event value from a stream of values. */
external sampleWith : stream 'sample => stream 'a => stream 'a = "" [@@bs.module "most"];

external zip : ('a => 'b => 'c) => stream 'a => stream 'b => stream 'c = "" [@@bs.module "most"];


/**
 * Combining higher-order streams
 **/
/* Given a higher-order stream, return a new stream that adopts the behavior of
   (ie emits the events of) the most recent inner stream. */
external switchLatest : stream (stream 'a) => stream 'a = "" [@@bs.module "most"];

/* Given a higher-order stream, return a new stream that merges all the inner streams as they arrive. */
external join : stream (stream 'a) => stream 'a = "" [@@bs.module "most"];

/* Given a higher-order stream, return a new stream that merges inner streams as they arrive
   up to the specified concurrency. Once concurrency number of streams are being merged,
   newly arriving streams will be merged after an existing one ends. */
external mergeConcurrently : int => stream (stream 'a) => stream 'a = "" [@@bs.module "most"];


/**
 * Awaiting promises
 **/
/* Given a stream of promises, ie Stream<Promise<X>>,
   return a new stream containing the fulfillment values, ie Stream<X>.
   Event times may be delayed. However, event order is always preserved,
   regardless of promise fulfillment order.
   To create a stream that merges promises in fulfillment order,
   use `flatMap(fromPromise, stream)`.
   If a promise rejects, the stream will be in an error state
   with the rejected promise's reason as its error.
   See recoverWith for error recovery. */
external awaitPromises : stream (Js.Promise.t 'a) => stream 'a = "" [@@bs.module "most"];


/**
 * Rate limiting streams
 */
/* Wait for a burst of events to subside and emit only the last event in the burst.
   If the stream ends while there is a pending debounced event (e.g. via `until`),
   the pending event will be emitted just before the stream ends. */
external debounce : int => stream 'a => stream 'a = "" [@@bs.module "most"];

/* Limit the rate of events to at most one per throttlePeriod. */
external throttle : int => stream 'a => stream 'a = "" [@@bs.module "most"];


/**
 * Delaying streams
 **/
/* Timeshift a stream by a delay time in milliseconds. */
external delay : int => stream 'a => stream 'a = "" [@@bs.module "most"];


/**
 * Sharing streams
 **/
/* Returns a stream equivalent to the original, but which can be shared more efficiently among multiple consumers. */
external multicast : stream 'a => stream 'a = "" [@@bs.module "most"];
