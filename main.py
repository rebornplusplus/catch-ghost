#!/usr/bin/python3

import pygame as pg
import random, math, sys, getopt
from settings import *
from sprites import *
from os import path

class Game:
	def __init__(self):
		# initialize game window, etc
		pg.init()
		pg.mixer.init()
		self.screen = pg.display.set_mode((WIDTH, HEIGHT))
		pg.display.set_caption(TITLE)
		self.clock = pg.time.Clock()
		self.running = True
		self.font_name = pg.font.match_font(FONT_NAME)
		self.load_data()

	def load_data(self):
		# load high score
		# load images
		# load sounds
		pass

	def write_data(self):
		# write high score
		pass

	def new(self):
		# start a new game
		# sprite groups
		self.all_sprites = pg.sprite.LayeredUpdates()
		self.bg = pg.sprite.Group()
		self.chunks = pg.sprite.Group()
		self.highlighters = pg.sprite.Group()
		# create text boxes
		self.shift_box = TextBox(self, 700, PLAYER_RADIUS * 4, BLACK, "Shift Time Frame", 150, FONT_SZ * 5)
		self.bust_box = TextBox(self, 700, HEIGHT - PLAYER_RADIUS * 4, BLACK, "BUST!", 150, FONT_SZ * 5)
		# click and time frame count
		self.click_cnt = 0
		self.time_frame = 0
		# create board and initiate
		self.create_board()
		# run game loop
		self.run()

	def run(self):
		# game loop
		self.playing = True
		while self.playing:
			self.clock.tick(FPS)
			self.events()
			self.update()
			self.draw()
		
	def update(self):
		# game loop - update
		self.all_sprites.update()

	def events(self):
		# game loop - events
		for event in pg.event.get():
			# check for closing window
			if event.type == pg.QUIT:
				if self.playing:
					self.playing = False
				self.running = False
				break
			# pause if Esc pressed
			if event.type == pg.KEYUP:
				if event.key == pg.K_ESCAPE:
					self.wait_for_key()
			# check for mouse click
			if event.type == pg.MOUSEBUTTONDOWN:
				if event.button == 1:
					# check if cell is clicked
					for chunk in self.chunks:
						if chunk.rect.collidepoint(event.pos):
							x, y = self.translate_back(*chunk.rect.center)
							self.click(x, y)
							break
					# check if time frame is to be shifted
					if self.shift_box.rect.collidepoint(event.pos):
						x, y = self.translate_back(*self.shift_box.rect.center)
						self.shift_time_frame()
						break
					# bust ghost
					if self.click_cnt > 0 and self.bust_box.rect.collidepoint(event.pos):
						self.bust(self.click_x, self.click_y)
	
	def draw(self):
		# game loop - draw
		self.draw_bg()
		for sprite in self.all_sprites:
			sprite.draw()
		self.draw_board()
		# *after* drawing everything, flip the display
		pg.display.flip()
		
	def wait_for_key(self):
		waiting = True
		while waiting:
			self.clock.tick(FPS)
			for event in pg.event.get():
				if event.type == pg.QUIT:
					waiting = False
					self.running = False
				if event.type == pg.KEYUP:
					waiting = False
			pg.display.flip()

	def show_start_screen(self):
		# game splash/start screen
		self.screen.fill(BGCOLOR)
		self.draw_text(TITLE, 50, BLACK, WIDTH / 2, HEIGHT / 4)
		self.draw_text("Press a key to start playing.", 18, BLACK, WIDTH / 2, 3 * HEIGHT / 4)
		pg.display.flip()
		self.wait_for_key()

	def show_go_screen(self):
		# game over/continue
		if not self.running:
			return

		if self.result == "OK":
			self.result = "CAUGHT!"
		else:
			self.result = "MISSED!"

		self.draw()
		self.draw_text(self.result, 40, BLACK, 700, HEIGHT // 2 - 20)
		pg.display.flip()
		self.wait_for_key()

		self.screen.fill(BGCOLOR)
		self.draw_text("Game Over!", 50, BLACK, WIDTH / 2, HEIGHT / 4)
		self.draw_text(self.result, 18, BLACK, WIDTH / 2, HEIGHT / 4 + 100)
		self.draw_text("Press a key to play again.", 18, BLACK, WIDTH / 2, 3 * HEIGHT / 4)
		pg.display.flip()
		self.wait_for_key()

	def draw_text(self, text, size, color, x, y):
		font = pg.font.Font(self.font_name, size)
		text_surface = font.render(text, True, color)
		text_rect = text_surface.get_rect()
		text_rect.midtop = (x, y)
		self.screen.blit(text_surface, text_rect)

	def translate_coords(self, x, y):
		return ( OFFSET + (x * 2 + 1) * PLAYER_RADIUS, OFFSET + (y * 2 + 1) * PLAYER_RADIUS )
	
	def translate_back(self, x, y):
		return ( ((x - OFFSET) // PLAYER_RADIUS - 1) // 2, ((y - OFFSET) // PLAYER_RADIUS - 1) // 2 )

	def draw_bg(self):
		self.screen.fill(BGCOLOR)
		for i in range(0, GRID_SZ):
			for j in range(0, GRID_SZ):
				col = DARKYELLOW
				if (i + j) % 2 == 1:
					col = LIGHTYELLOW
				x, y = self.translate_coords(i, j)
				BoardChunk(self, x, y, col)
		for i in range(0, GRID_SZ):
			x, y = self.translate_coords(-1, i)
			self.draw_text(str(GRID_SZ - i), FONT_SZ, BLACK, x, y)
			x, y = self.translate_coords(GRID_SZ, i)
			self.draw_text(str(GRID_SZ - i), FONT_SZ, BLACK, x, y)
			x, y = self.translate_coords(i, -1)
			self.draw_text(chr(ord('A') + i), FONT_SZ, BLACK, x, y)
			x, y = self.translate_coords(i, GRID_SZ)
			self.draw_text(chr(ord('A') + i), FONT_SZ, BLACK, x, y)

	def create_board(self):
		# create a 2d matrix
		self.board = [[0 for x in range(GRID_SZ)] for y in range(GRID_SZ)]
		# do a reset call
		print("RESET {}".format(GRID_SZ))
		sys.stdout.flush()
		# input the board
		for i in range(GRID_SZ):
			self.board[i] = list(map(float, input().split()))
		# print(self.board)
		# sys.stdout.flush()
	
	def draw_board(self):
		for i in range(GRID_SZ):
			for j in range(GRID_SZ):
				x, y = self.translate_coords(i, j)
				y -= FONT_SZ // 2
				self.draw_text(str(self.board[i][j]), FONT_SZ, BLACK, x, y)
	
	def click(self, x, y):
		self.click_x = x
		self.click_y = y
		self.click_cnt += 1
		
		print("CLICK {} {}".format(self.click_x, self.click_y))
		sys.stdout.flush()

		color_str = input()
		for i in range(GRID_SZ):
			self.board[i] = list(map(float, input().split()))

		color = DARKGREEN
		if color_str == "RED":
			color = RED
		elif color_str == "ORANGE":
			color = DARKORANGE
		else:
			color = DARKGREEN
		
		for highlighter in self.highlighters:
			highlighter.kill()
		Highlighter(self, *self.translate_coords(self.click_x, self.click_y), color)
	
	def shift_time_frame(self):
		self.time_frame += 1

		print("SHIFT")
		sys.stdout.flush()

		for i in range(GRID_SZ):
			self.board[i] = list(map(float, input().split()))

		for highlighter in self.highlighters:
			highlighter.kill()
	
	def bust(self, x, y):
		print("BUST {} {}".format(x, y))
		sys.stdout.flush()

		self.result = input()
		self.playing = False
	
if __name__ == "__main__":
	g = Game()
	g.show_start_screen()
	while g.running:
		g.new()
		g.show_go_screen()
	print("QUIT")
	sys.stdout.flush()
	pg.quit()
