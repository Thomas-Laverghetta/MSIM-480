@echo off 
FOR /L %%i IN (4,1,6) DO (
  C:\Python38\python.exe .\Assignment1.py %%i DFS
)
FOR /L %%i IN (4,1,6) DO (
  C:\Python38\python.exe .\Assignment1.py %%i BFS
)
FOR /L %%i IN (4,1,8) DO (
  C:\Python38\python.exe .\Assignment1.py %%i GBS
)
FOR /L %%i IN (4,1,8) DO (
  C:\Python38\python.exe .\Assignment1.py %%i Astar
)