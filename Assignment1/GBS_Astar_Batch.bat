@echo off 
FOR /L %%i IN (4,1,10) DO (
  C:\Python38\python.exe .\Assignment1.py %%i GBS
)
FOR /L %%i IN (4,1,10) DO (
  C:\Python38\python.exe .\Assignment1.py %%i Astar
)