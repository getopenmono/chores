# Chores

This is a [Mono](http://openmono.com) application for keeping track of the small mundane tasks that have to be done once in a while.

Mono will display an icon for each task and display how long since you or someone else did the task.  Touching the number will reset the count.

You can display up to three tasks on the screen.  To replace a task with another task, click on the icon you want to replace, and the row switches to a list of all available icons, which you can now browse through, and click to select.

## Icons

Conversion of icons is done by

    for f in ~/repos/chores/icons/*.png; do ./img2icon -s 64,64 $f; done
    for f in ~/repos/chores/icons/arrow-*.png; do ./img2icon -s 88,28 $f; done

