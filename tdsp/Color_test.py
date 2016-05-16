import unittest

from tdsp import Color, Color256

class TestColor(unittest.TestCase):
    def test_white(self):
        white = Color(1, 1, 1)
        self.assertEqual(str(white), 'white')
        self.assertEqual(white, Color((1, 1, 1)))
        self.assertEqual(white[:], white)
        self.assertEqual(white[::-1], white)
        self.assertEqual(white[0:2], (1.0, 1.0))

    def test_red(self):
        red = Color(1, 0, 0)
        blue = Color('blue')

        self.assertEqual(str(red), 'red')
        self.assertEqual(red[:], red)
        self.assertEqual(red[::-1], blue)
        self.assertEqual(red[-3:3:1], red)
        self.assertEqual(red[-1:-3:-1], (0.0, 0.0))
        self.assertEqual(red[-3:-1], (1.0, 0.0))
        self.assertEqual(red[0:2], (1.0, 0.0))

    def test_mixed(self):
        antired = Color(-1, 0, 0)
        self.assertEqual(str(antired), 'red-++')
        self.assertEqual(antired + Color('red'), Color('black'))
        self.assertEqual(str(-Color('white')), 'white---')

    def test_arithmetic(self):
        black, red, green, blue, white, yellow, cyan, magenta = (
            Color(i) for i in ('black', 'red', 'green', 'blue', 'white',
                               'yellow', 'cyan', 'magenta'))

        self.assertEqual(red + green + blue, white)
        self.assertEqual(red + green, yellow)
        self.assertEqual(green + blue, cyan)
        self.assertEqual(red + blue, magenta)

        self.assertEqual(white - blue, yellow)
        self.assertEqual(white - green, magenta)
        self.assertEqual(white - red, cyan)

        self.assertEqual(~white, black)
        self.assertEqual(~black, white)

        self.assertEqual(white * red, red)
        self.assertEqual(white * cyan, cyan)

    def test_methods(self):
        self.assertEqual(Color(1.2, -3, 5.0).normalized(),
                         Color('red') + Color('blue'))
        self.assertEqual(Color().ratio, 1.0)

    def test_rotated(self):
        self.assertEqual(Color('red').rotated(1), Color('blue'))
        self.assertEqual((Color('red') + Color('blue') * 0.5).rotated(-1),
                          Color('green') + Color('red') * 0.5)

    def test_compare(self):
        for x in 0, -1, 'red', (0.1, 0.1, 0.1):
            x = Color(x)
            self.assertTrue(x == x)
            self.assertFalse(x != x)
            self.assertTrue(x <= x)
            self.assertFalse(x < x)
            self.assertTrue(x >= x)
            self.assertFalse(x > x)

        black, red, green, blue, white, yellow, cyan, magenta = (
            Color(i) for i in ('black', 'red', 'green', 'blue', 'white',
                               'yellow', 'cyan', 'magenta'))
        for x, y in ((red, green), (green, blue), (red + green, red + blue),
                     (white, red), (white, green), (white, blue)):
            self.assertFalse(x == y)
            self.assertTrue(x != y)
            self.assertFalse(x <= y)
            self.assertFalse(x < y)
            self.assertTrue(x >= y)
            self.assertTrue(x > y)

    def test_indexing(self):
        c = Color(1, 2, 3)
        self.assertEqual(c[0], 1)
        self.assertEqual(c[1], 2)
        self.assertEqual(c[2], 3)
        self.assertEqual(c[-3], 1)
        self.assertEqual(c[-2], 2)
        self.assertEqual(c[-1], 3)
        with self.assertRaises(IndexError):
            c[-4]
        with self.assertRaises(IndexError):
            c[3]

        r, g, b = c
        self.assertEqual(c, Color(r, g, b))
        self.assertEqual(c, Color(c))
        self.assertEqual(c, Color(*c))

    def test_abs(self):
        red, green, gray = (
            Color('red'), Color('green'), Color('gray'))
        self.assertEqual(abs(-red), red)
        self.assertEqual(abs(-(red + gray + gray)), red + gray + gray)


class TestColor256(unittest.TestCase):
    def test_first(self):
        white = Color256(255, 255, 255)
        self.assertEqual(str(white), 'white')
        self.assertEqual(white[:], white)
        self.assertEqual(white[::-1], white)
        self.assertEqual(white[0:2], (255, 255))

    def test_red(self):
        red = Color256((255, 0, 0))
        blue = Color256('blue')

        self.assertEqual(str(red), 'red')
        self.assertEqual(red[:], red)
        self.assertEqual(red[-1:-3:-1], (0.0, 0.0))
        self.assertEqual(red[-3:-1], (255, 0.0))
        self.assertEqual(red[0:2], (255, 0.0))
        self.assertEqual(red[-3:3:1], red)
        self.assertEqual(red[::-1], blue)

    def test_arithmetic(self):
        black, red, green, blue, white, yellow, cyan, magenta = (
            Color256(i) for i in ('black', 'red', 'green', 'blue', 'white',
                                  'yellow', 'cyan', 'magenta'))

        self.assertEqual(red + green + blue, white)
        self.assertEqual(red + green, yellow)
        self.assertEqual(green + blue, cyan)
        self.assertEqual(red + blue, magenta)

        self.assertEqual(white - blue, yellow)
        self.assertEqual(white - green, magenta)
        self.assertEqual(white - red, cyan)

        self.assertEqual(~white, black)
        self.assertEqual(~black, white)

        self.assertEqual(white * red / 255, red)
        self.assertEqual(white * cyan / 255, cyan)

    def test_methods(self):
        self.assertEqual((Color256('red') + Color256('blue') * 0.5).rotated(-1),
                          Color256('green') + Color256('red') * 0.5)
        self.assertEqual(Color256('red').rotated(1), Color256('blue'))
        self.assertEqual(Color256(300, -500, 1000).normalized(),
                         Color256('red') + Color256('blue'))
        self.assertEqual(Color256().ratio, 255.0)

    def test_names(self):
        names = Color.names()
        self.assertEqual(len(names), 481)
        self.assertEqual(names[:10], [
            'alice blue',
            'antique white',
            'antique white 1',
            'antique white 2',
            'antique white 3',
            'antique white 4',
            'aqua',
            'aquamarine',
            'aquamarine 1',
            'aquamarine 2'])
