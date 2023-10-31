# HW 2
John Adams A16499049

I did not do any of the bonus points, but I did implement a lazy z-buffer occlusion (doesn't use the hierarchy, checks per-pixel) and a 2D-BoundingBox acceleration into 2.4 to speed up rendering. (It's about 106x faster than without those two accelerations)

## 2.5 Design a Scene

scene: `scenes/hw2/luigi.json`

image: `outputs/luigi.png`

Honestly It took me like 2 hours to figure out how to map textures to vertex coordinates in blender so I don't think I have the patience to set up a proper scene. So you get Luigi.

Credits to Nintendo (because he is ripped directly out of Super Mario Bros. Wonder) I did some modifications: surface subdivision on his eyes and hat to improve the texture rendering in those locations, and of course converting the textures to vertex colors (although they are quite crude looking).

He has 26417 Vertices and I got it to render in only 0.175 seconds on a base M1 Mac, so I think that's pretty optimized if I do say so myself.

Similarly the teapot renders in 0.048 seconds in my implmentation.


#  HW 1
John Adams A16499049


## 2 Bonus (Accelerate with Bounding Boxes)
I did implement a bounding box rendering accelerator
(can be turned on by compiling with `boundingBox = 1`).

I did not see very large speedup, although I assume this has more to do with the very small number of objects that were drawn for each scene. These are the render time differences for each scene.

0: 0.0034 -> 0.0030

1: 0.0122 -> 0.0104

2: 0.0051 -> 0.0032

3: 0.0043 -> 0.0038

4: 0.0061 -> 0.0055

---

Side note: I also implemented the bounding box rendering for part 3 because I thought we were supposed to lol. I dropped it for 4+ because it took more code to implement.

## 4 Bonus (Animation)
I attempted this, but was unable to do it. For whatever reason (something to do with the Linker and CMake I think) I wasn't able to make a global variable to handle the animation progress (I was going to implement it within the transform parsing). I didn't want to rewrite all the function headers to implement some kind of progress variable so I gave up on that.


## 7 (Design a Scene)
I made a holiday phone background. It has a moon, some rolling hills, a pine tree and a present box under snowfall.

scene: `scenes/hw1/holiday.json`

image: `outputs/holiday.png`

I hope you like it :)


## 8 Bonus (Lines & Curves)
I did not do this bonus. Have a lot of work this weekend. If I finish early than I suppose I might resubmit.