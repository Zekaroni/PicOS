from PIL import Image, ImageDraw, ImageFont

FONT_NAME = "Minecraftia-Regular.ttf"


def generate_cpp_font(ttf_path, font_size, chars):
    outputString = "" 
    font = ImageFont.truetype(ttf_path, font_size)
    master_img = Image.new('1', (8 * len(chars), 8), color=0)

    
    outputString += "const uint8_t minecraftia_regular_font[][8] = {\n"
    
    for i, char in enumerate(chars):
        img = Image.new('1', (8, 8), color=0)
        draw = ImageDraw.Draw(img)
        draw.text((0, 12), char, font=font, fill=1, anchor="ls")
        master_img.paste(img, (i * 8, 0))
        
        outputString+="    {"
        for y in range(8):
            byte_val = 0
            for x in range(8):
                if img.getpixel((x, y)) > 0:
                    byte_val |= (1 << (7 - x))
            outputString += f" 0b{byte_val:08b},"
            
        outputString += " },"
        outputString += f" // {char}\n"
        
    outputString += "};"
    master_img.save("font_debug.png")

    with open("rendered_font_obj.cpp", "w") as file:
        file.write(outputString)

generate_cpp_font(FONT_NAME, 8, r" !\"#$%&'()*+'-./0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ[\]^_`abcdefghijklmnopqrstuvwxyz{|}~")