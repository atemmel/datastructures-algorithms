# Laboration 4: Box blur

## 1. About
Box blur is an algorithm used to apply a blur effect to an image. It takes a texture as input and, optionally, a parameter indicating the radius of
the resulting blur. The output of the algorithm is the same texture, but with a blur effect applied to it. The complexity of this algorithm is
*O(nr²)*  where *n* is the amount of pixels to process, and *r* is the radius of the blur effect.

## 2. Implementation
```
function boxblur( texture, radius )
	x ← 0
	y ← 0
	offset ← radius / 2

	while( x < texture.width )
		while( y < texture.height )
			color ← [0, 0, 0]
			i ← -offset
			j ← -offset
			iterations ← 0

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

			mean = color / iterations
			texture[x][y] = mean

			y ← y + 1
		x ← x + 1
```

Above is an approximate implementation of the box blur algorithm. To properly understand the algorithm, you really only need to grasp one single 
operation, as the rest of the algorithm is just iterations to apply this operation on each pixel inside of the texture.

## 3. Competitors
