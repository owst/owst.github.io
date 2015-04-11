---
title: AES using ECB Demo using Python
sub_title: A comparison between ECB and CBC in the AES algorithm using Python
---
Here's a little python program that I've knocked together inspired by
[this][inspired_by] article, to demonstrate the serious gotcha involved with
using [ECB (Electronic Code Book)][ecb] as a cipher mode.

This code uses the [PIL (Python Imaging Library)][pil] (I used v1.1.6) and
[PyCrypto][pycrypto] (I used the precompiled binary from
[here][pycrypto_binary]).

These modules are designed for Python 2.7, so that's what I used.

The code:

```python
import Image
import sys
import os
from Crypto.Cipher import AES

    IV_SIZE = 16
    BLOCK_SIZE = 16

    def check_args():
        try:
            if (len(sys.argv) != 4):
                raise Exception()
            elif (not os.path.isfile(sys.argv[1])):
                raise Exception("Input file must exist")
            elif (not sys.argv[3] in ['CBC', 'ECB']):
                raise Exception("Block cipher mode should be ECB or CBC")
            return (sys.argv[1], sys.argv[2], sys.argv[3])
        except Exception, ex:
            print "Usage:", sys.argv[0], \
              "full_path_to_input_image full_path_to_output_image ECB|CBC"
            if len(ex.args) > 0:
                print "--" + str(ex)
            sys.exit(1)


    def encrypt(input_filename, output_filename, cipher_mode):
        """Encrypt an image file and write out the results as a JPEG."""

        input_image = Image.open(input_filename)

        # Key must be one of 16/24/32 bytes in length.
        key = "0123456789ABCDEF"
        mode = AES.MODE_CBC if cipher_mode == 'CBC' else AES.MODE_ECB
        iv = os.urandom(IV_SIZE)

        aes = AES.new(key, mode, iv)

        image_string = input_image.tostring()
        # The input string must be padded to the input block size.
        image_padding_length = BLOCK_SIZE - len(image_string) % BLOCK_SIZE
        image_string += image_padding_length * "~"

        # generate the encrypted image string
        encrypted = aes.encrypt(image_string)

        # create an image from the encrypted string
        encrypted_img = Image.frombuffer("RGB", input_image.size, encrypted, 'raw',
                                         "RGB", 0, 1)

        # create and save the output image
        encrypted_img.save(output_filename, 'JPEG')

        print("Encrypted using AES in " + cipher_mode + " mode and saved to \"" +
               output_filename + "\"!")


    if __name__ == "__main__":
        args = check_args()
        encrypt(*args)
```
And the test image, and resulting encrypted image, using first ECB then [CBC
(Cipher Block Chaining)][cbc].

Input:
{% capture img_url %}{{site.img_dir}}{{page.categories}}/{{page.date | date: '%Y-%m-%d'}}{% endcapture %}
![test-input]({{img_url}}/test.jpg)
Encrypted with ECB:
![test-ecb]({{img_url}}/test_ecb.jpg)
Encrypted with CBC:
![test-cbc]({{img_url}}/test_cbc.jpg)


As you can see, ECB outputs the same encrypted data for equivalent input
blocks, so the black and white areas are clearly distinguishable. Since CBC has
feedback from the previous block's encryption into the next block, the
equivalent input blocks are no longer encrypted to the same value, so we see
noise.

[inspired_by]: http://chargen.matasano.com/chargen/2009/7/22/if-youre-typing-the-letters-a-e-s-into-your-code-youre-doing.html
[ecb]: http://en.wikipedia.org/wiki/Block_cipher_modes_of_operation#Electronic_codebook_.28ECB.29
[cbc]: http://en.wikipedia.org/wiki/Block_cipher_modes_of_operation#Cipher-block_chaining_.28CBC.29
[pil]: http://www.pythonware.com/products/pil/
[pycrypto]: http://www.dlitz.net/software/pycrypto/
[pycrypto_binary]: http://www.voidspace.org.uk/python/modules.shtml#pycrypto
