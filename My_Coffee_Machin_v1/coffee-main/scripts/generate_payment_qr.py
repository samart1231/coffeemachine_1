import qrcode
from PIL import Image
import sys

def create_and_show_qr_code(data):
    try:
        qr = qrcode.QRCode(
            version=1,
            error_correction=qrcode.constants.ERROR_CORRECT_L,
            box_size=10,
            border=4,
        )
        qr.add_data(data)
        qr.make(fit=True)
        img = qr.make_image(fill_color="black", back_color="white")
        img.show()
        print("QR code displayed successfully.")
    except Exception as e:
        print(f"Error: {e}")

if len(sys.argv) != 2:
    print("Usage: python generate_payment_qr.py <cart_file>")
    sys.exit(1)

cart_file = sys.argv[1]

try:
    with open(cart_file, 'r') as file:
        cart_data = file.read()
    create_and_show_qr_code(cart_data)
except Exception as e:
    print(f"Error: {e}")