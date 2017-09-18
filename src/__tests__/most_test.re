open Jest;

module Helpers = {
  let noOp _ => ();
  let combineArray coll el => Js.Array.push el coll |> noOp;
  let combineList collRef el => (collRef := [el, ...!collRef]) |> noOp;
  let asyncExpectToEqual expected actual =>
    Js.Promise.resolve Expect.(expect actual |> toEqual expected);
};


/**
 * Stream creation
 **/
testPromise
  "empty"
  (
    fun _ => {
      let empty = Most.empty ();
      let success _ => Helpers.asyncExpectToEqual true true;
      Most.observe (fun _ => raise (Failure "this shouldn't happen")) empty |>
      Js.Promise.then_ success
    }
  );

testAsync
  "just"
  (
    fun done_ => {
      open Expect;
      let success result => done_ (expect result |> toBe "asdf");
      Most.just "asdf" |> Most.observe success |> Helpers.noOp
    }
  );

testPromise
  "from"
  (
    fun _ => {
      let result = [||];
      let combine = Helpers.combineArray result;
      let success _ => Helpers.asyncExpectToEqual [|1, 2|] result;
      Most.from [|1, 2|] |> Most.observe combine |> Js.Promise.then_ success
    }
  );

testPromise
  "fromPromise"
  (
    fun _ => {
      let promise = Js.Promise.resolve "OK";
      let result = ref "";
      let success = Helpers.asyncExpectToEqual "OK";
      Most.fromPromise promise |> Most.observe (fun res => result := res) |>
      Js.Promise.then_ (fun _ => success !result)
    }
  );

testPromise "periodic";

testPromise "never";

testPromise "iterate";

testPromise "iteratePromise";

testPromise "fromEvent";

testPromise
  "concat"
  (
    fun _ => {
      let result = [||];
      let combine = Helpers.combineArray result;
      let s1 = Most.from [|1, 2, 3|];
      let s2 = Most.from [|4, 5, 6|];
      let success _ => Helpers.asyncExpectToEqual [|1, 2, 3, 4, 5, 6|] result;
      Most.concat s2 s1 |> Most.observe combine |> Js.Promise.then_ success
    }
  );

testPromise
  "startWith"
  (
    fun _ => {
      let result = [||];
      let combine = Helpers.combineArray result;
      let s = Most.from [|1, 2, 3|];
      let success _ => Helpers.asyncExpectToEqual [|0, 1, 2, 3|] result;
      Most.startWith 0 s |> Most.observe combine |> Js.Promise.then_ success
    }
  );

testPromise
  "unfold"
  (
    fun _ => {
      let result = ref [];
      let combine el => (result := [el, ...!result]) |> Helpers.noOp;
      let success _ => Helpers.asyncExpectToEqual [6, 4, 2] !result;
      Most.unfold
        (
          fun seed =>
            if (seed < 4) {
              let nextSeed = seed + 1;
              Some (seed * 2, nextSeed)
            } else {
              None
            }
        )
        1 |>
      Most.observe combine |>
      Js.Promise.then_ success
    }
  );

testPromise
  "fromList"
  (
    fun _ => {
      let result = ref [];
      let combine = Helpers.combineList result;
      /* combineList adds them to the list in reverse order */
      let success _ => Helpers.asyncExpectToEqual [3, 2, 1] !result;
      Most.fromList [1, 2, 3] |> Most.observe combine |> Js.Promise.then_ success
    }
  );


/**
 * Error handling
 **/
testPromise "recoverWith";

testPromise "throwError";


/**
 * Transforming
 **/
testPromise
  "map"
  (
    fun _ => {
      let result = [||];
      let combineResult = Helpers.combineArray result;
      let success _ => Helpers.asyncExpectToEqual [|2, 4, 6|] result;
      Most.(
        from [|1, 2, 3|] |> map (fun n => n * 2) |> observe combineResult |>
        Js.Promise.then_ success
      )
    }
  );

testPromise
  "constant"
  (
    fun _ => {
      let result = [||];
      let combine = Helpers.combineArray result;
      let success _ => Helpers.asyncExpectToEqual [|10, 10, 10|] result;
      Most.from [|1, 2, 3|] |> Most.constant 10 |> Most.observe combine |> Js.Promise.then_ success
    }
  );

testPromise
  "scan"
  (
    fun _ => {
      let result = [||];
      let combine = Helpers.combineArray result;
      let success _ => Helpers.asyncExpectToEqual [|0, 1, 3, 6, 10|] result;
      Most.from [|1, 2, 3, 4|] |> Most.scan (fun accum n => accum + n) 0 |> Most.observe combine |>
      Js.Promise.then_ success
    }
  );

testPromise
  "flatMap"
  (
    fun _ => {
      let result = [||];
      let combine = Helpers.combineArray result;
      let success _ => Helpers.asyncExpectToEqual [|0, 1, 0, 2, 0, 3, 0, 4|] result;
      Most.from [|1, 2, 3, 4|] |> Most.flatMap (fun x => Most.from [|0, x|]) |>
      Most.observe combine |>
      Js.Promise.then_ success
    }
  );

testPromise "continueWith";

testPromise "concatMap";

testPromise "ap";

testPromise "timestamp";

testPromise "tap";


/**
 * Filtering
 **/
testPromise
  "filter"
  (
    fun _ =>
      Most.from [|1, 2, 3, 4, 5, 6|] |>
      Most.filter (
        fun n =>
          if (n mod 2 === 0) {
            Js.true_
          } else {
            Js.false_
          }
      ) |>
      Most.reduce (fun acc n => [n, ...acc]) [] |>
      Js.Promise.then_ (
        fun result => Js.Promise.resolve Expect.(expect result |> toEqual [6, 4, 2])
      )
  );

testPromise "skipRepeats";

testPromise "skipRepeatsWith";


/**
 * Slicing
 **/
testPromise "slice";

testPromise
  "take"
  (
    fun _ =>
      Expect.(
        Most.(from [|1, 2, 3, 4, 5, 6|] |> take 3 |> reduce (fun acc n => [n, ...acc]) []) |>
        Js.Promise.(then_ (fun result => resolve (expect result |> toEqual [3, 2, 1])))
      )
  );

testPromise "skip" (fun _ => 
  Expect.(Most.(from [|1,2,3,4,5,6|] |> skip 3 |> reduce (fun acc n => [n, ...acc]) []) |>
    Js.Promise.(then_ (fun result => resolve (expect result |> toEqual [6,5,4])))));

testPromise "takeWhile";

testPromise "skipWhile";

testPromise "skipAfter";

testPromise "until";

testPromise "since";

testPromise "during";


/**
 * Combining
 **/
testPromise "merge";

testPromise "combine";

testPromise "zip";

testPromise "merge";

testPromise "combine";

testPromise "sample2";

testPromise "sample3";

testPromise "sample4";

testPromise "sample5";

testPromise "sample6";

testPromise "sampleWith";

testPromise "zip";


/**
 * Combining higher-order streams
 **/
testPromise "switchLatest";

testPromise "join";

testPromise "mergeConcurrently";


/**
 * Awaiting promises
 **/
testPromise "awaitPromises";


/**
 * Rate limiting streams
 */
testPromise "debounce";

testPromise "throttle";


/**
 * Delaying streams
 **/
testPromise "delay";


/**
 * Sharing streams
 **/
testPromise "multicast";
