
## Game Developer test for King (C++)

Our goal is to provide a relevant, fun and inspirational foundation for a technical conversation in an upcoming interview. The submission itself together with the interview will provide the reviewer with an understanding of your coding style and skills.

At a minimum the features specified in this document should be delivered. Additional features subject to creativity and ambition are welcome.

We will evaluate both your code and your delivery.

### Concept

* The game consists of a 8x8 grid with different colored objects.
* There are 5 different object colors.
* Players can both drag and click objects to swap them.
* The objects can swap place as long as they are positioned next to each other and on the same row or column (up, down, left, right).
* If a switch result in 3 or more object of the same color in a row or column, these objects are removed from the grid.
* If a switch does not result in a match, the objects automatically switch back.
* When objects are removed from the grid, objects positioned above the removed ones fall down and new objects fall in from the top to fill the board.
* Any new matches created by the falling objects should be resolved and the previous step should be repeated until the board has no remaining matches.
* Each game round lasts 1 minute long.
* Use the game Midas Miner as a reference: http://www.royalgames.com/games/puzzle-games/miner-speed/

### Generating the project

To begin, download and install cmake with version >= 3.8.0 from https://cmake.org/download/

Next, open your commandline and change directory to the `worktest/` directory of this repository.

---

On **OSX**, use the following command to generate an Xcode project:

`cmake -G "Xcode"`

Open the produced `worktest.xcodeproj` and set "worktest" as the active scheme via the application menu Product -> Scheme.

---

On **Windows**, use the following command to generate a Visual Studios project:

`cmake .`

Open the produced `worktest.sln` and set "worktest" as the StartUp project.

---

To list all available project generators, run the following from your commandline:

`cmake -G`

### Starting point

Under `external/king/` an engine providing basic game development abstractions on top of SDL2 is provided.

A starting point for the match-3 game you are asked to implement is also provided under `src/`.

The starting point demonstrates model-view separation and initial work highlighting matches on an 8x8 grid of gems.

You can make use of or modify this starting point as you see fit.

### Submitting

* Please send us a ZIP archive or a link to where we can download one. It should contain your source code under `worktest/src/`, as well as an executable that runs out of the box on Windows or OSX.
* We also want to compile your submitted code, so please preserve the provided directory structures and cmake scripts.

* A readme.txt file to explain your architectural/design decisions is recommended.

### Evaluation

* We will look at your source code to evaluate:
  * Code structure and design choices, e.g.:
    * Separation of concerns
    * Readability and reusability
    * Extensibility for future features
    * The organisation of source files
  * Usage of language features, e.g.:
    * Memory management facilities
    * OOP facilities
  * Performance considerations, though we do not expect optimisation for highly resource-constrained scenarios
* Evaluate how the test performs against the specification.
* Evaluate the ”feeling” when playing the game i.e. Attention to transitions, animations and other details that might enhance the user experience.
* Look at the delivery itself with regards to structure, size, and whether or not it runs out of the box.

### Timeframe

* Please do your best to return the test within 7 days from receiving it. If you need more time please notify us.
