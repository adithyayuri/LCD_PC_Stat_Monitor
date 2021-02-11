echo on
echo "Build started\n"
echo ""
echo "Requirements"
echo ":: python > 3.5"
echo ":: pyinstaller module"
echo ":: pyserial module"
echo ""

pyinstaller --onefile host_pc_app/data_host.py

pause