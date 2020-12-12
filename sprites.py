import pygame as pg
from pygame import gfxdraw
from settings import *
vec = pg.math.Vector2

class BoardChunk(pg.sprite.Sprite):
	def __init__(self, game, x, y, color, thickness=0):
		self._layer = BG_LAYER
		self.groups = game.all_sprites, game.bg, game.chunks
		pg.sprite.Sprite.__init__(self, self.groups)

		self.game = game
		self.x = x
		self.y = y
		self.color = color
		self.thickness = thickness
		self.rect = pg.Rect((0, 0), (2 * PLAYER_RADIUS, 2 * PLAYER_RADIUS))
		self.rect.center = (self.x, self.y)
	
	def draw(self):
		pg.draw.rect(self.game.screen, self.color, self.rect, self.thickness)

class TextBox(pg.sprite.Sprite):
	def __init__(self, game, x, y, color, text, width=2*PLAYER_RADIUS, height=2*PLAYER_RADIUS, thickness=2):
		self._layer = BG_LAYER
		self.groups = game.all_sprites, game.bg
		pg.sprite.Sprite.__init__(self, self.groups)

		self.game = game
		self.x = x
		self.y = y
		self.color = color
		self.text = text
		self.width = width
		self.height = height
		self.thickness = thickness
		self.rect = pg.Rect((0, 0), (width, height))
		self.rect.center = (self.x, self.y)
	
	def draw(self):
		pg.draw.rect(self.game.screen, self.color, self.rect, self.thickness)
		self.game.draw_text(self.text, FONT_SZ, BLACK, self.x, self.y - FONT_SZ // 2)

class Highlighter(pg.sprite.Sprite):
	def __init__(self, game, x, y, color=RED, thickness=4):
		self._layer = HL_LAYER
		self.groups = game.all_sprites, game.highlighters
		pg.sprite.Sprite.__init__(self, self.groups)

		self.game = game
		self.x = x
		self.y = y
		self.color = color
		self.thickness = thickness
		self.rect = pg.Rect((0, 0), (2 * PLAYER_RADIUS, 2 * PLAYER_RADIUS))
		self.rect.center = (self.x, self.y)
	
	def draw(self):
		pg.draw.rect(self.game.screen, self.color, self.rect, self.thickness)
