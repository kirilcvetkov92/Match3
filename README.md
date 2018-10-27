

## Project version notes
--- 
### Architectural design decisions
* StateMachine : Added because of multiple states that UI can have, The game have several states and for easier controll, and avoiding recoursive game flow I dicedided to implement finite state automation to handle with it, also it handle the user swap and click controll.

* Actions :
  * Added timing *function callbacks* for handling events that could happen after some period of time.
   * Examples : The game should exit after 60 seconds, the game should be ready after 3 seconds e.t.c...
   *  Added *MoveTo action* for views, and it can accept sequence of MoveTo destinations that allow the view to move smoothly to specified position for specified ammount of time.
  
* I also added some functionalities such as:
  * setVisible
  * ConvertToNodeSpace
  * ConvertToWorldSpace

### Environment Xcode 9.2 
Used Time/Memory profilers

### Execution file 
The executable can be found in "Worktest/Debug/worktest"

### Concerns
I had no time to optimize the game to the state-of-the-art big(O) complexity.
There are some naive searches that could be speeded-up, but since the constraints are small, I decreased the priority of optimiziation. 
