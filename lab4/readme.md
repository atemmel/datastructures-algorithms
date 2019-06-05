adte1700, Datastructures and Algorithms

# Laboration 4: Box blur

## 1. About
Box blur is an algorithm used to apply a blur effect to an image. It takes a texture as input and, optionally, a parameter indicating the radius of
the resulting blur. The output of the algorithm is the same texture, but with a blur effect applied to it. The complexity of this algorithm is
*O(Nr²)*  where *N* is the amount of pixels within the texture to process, and *r* is the chosen radius of the blur effect.

## 2. Implementation
```cpp
function boxblur( texture, radius )
	x ← 0
	y ← 0
	offset ← radius / 2
	//For each pixel
	while( x < texture.width )
		while( y < texture.height )
			color ← [0, 0, 0]
			i ← -offset
			j ← -offset
			iterations ← 0
			//Sum all the pixels within our radius
			while( i < offset )
				if ( i < 0 or i >= texture.width ) 
					i ← i + 1
					continue
				while( j < offset )
					if( j < 0 or j >= texture.height ) 
						j ← j + 1
						continue
					color ← color + texture[x + i][y + j]
					iterations += 1
					j ← j + 1
				i ← i + 1
			//Calculate the mean
			mean = color / iterations
			texture[x][y] = mean

			y ← y + 1
		x ← x + 1
```

Above is an approximate implementation of the box blur algorithm. To properly understand the algorithm, you really only need to grasp one single 
operation, as the rest of the algorithm is just iterations to apply this operation on each pixel inside of the texture.

![](1iter.png)
![](3iter.png)
![](5iter.png)
Repeated usage of the box blur algorithm on the same texture can also improve the end blur.
Above is a comparison between one, three and five iterations respectively. The image gets noticeably more blurry as the iterations start to ramp up.


## 3. Competitiors

The main competitor to box blur is gaussian blur. It works a little bit differently, but still tries to achieve the same goal. Instead of taking the
mean, it operates using he normal distribution for calculating the transformation for each pixel. Thanks to the added complexity of this step, 
gaussian blur turns out to be less performant than the standard box blur, landing at the same theoretical complexity *O(Nr²)* but due to the fact 
that each individual operation is more expensive, the end result ends up being more performant. 

One could, however, make the argument that the 
resulting effect from a gaussian blur is not visually identical to a box blur, meaning that depending on the situation, one may be preferred over the
other due to visual appeal alone.

## 4. References

[Wojciech Jarosz. 2001. Fast Image Convolutions](https://web.archive.org/web/20060718054020/http://www.acm.uiuc.edu/siggraph/workshops/wjarosz_convolution_2001.pdf)
