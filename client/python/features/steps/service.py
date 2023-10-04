from base64 import b64encode, b64decode
from behave import given, then, step
from behave.api.async_step import async_run_until_complete
from behave.runner import Context
from hamcrest import assert_that, equal_to, not_

from client import Client as _Client


@given("The encrypter service running on {host} on port {port:d}")
def step_impl(context: Context, host: str, port: int):
    context.host = host
    context.port = port
    context.value = "0ENKfo4NkIAHVBoNUtUseShHK/6JQ11YGALUSA/RLcICLQaTyxDJcldEsxxM0h+AWdkXAXNY6xOhCk+TkobTayscjTPqu6bzKIxnD1hso8Hz/ZWsh4SlSxHLhHAZRg6VfBZlITexMDdMOks2eauDdND6ft5kT+7uJFpq5EfnuHE="


@then("Encrypt a string")
@async_run_until_complete
async def step_impl(context: Context):
    assert_that(hasattr(context, "host"), equal_to(True), "Encrypter service host not set")
    assert_that(hasattr(context, "port"), equal_to(True), "Encrypter service port not set")
    assert_that(hasattr(context, "value"), equal_to(True), "String to encrypt not set")

    async with _Client(host=context.host, port=context.port) as client:
        value = await client.encrypt(context.value)
        assert_that(len(value), not_(equal_to(0)), "Encrypted value of zero length")
        context.encrypted = value


@step("Decrypt the encrypted string")
@async_run_until_complete
async def step_impl(context: Context):
    assert_that(hasattr(context, "host"), equal_to(True), "Encrypter service host not set")
    assert_that(hasattr(context, "port"), equal_to(True), "Encrypter service port not set")
    assert_that(hasattr(context, "value"), equal_to(True), "String to encrypt not set")
    assert_that(hasattr(context, "encrypted"), equal_to(True), "Encrypted string not set")

    async with _Client(host=context.host, port=context.port) as client:
        value = await client.decrypt(context.encrypted)
        assert_that(value, equal_to(context.value), "Decrypted value not same as original")


@step("Encrypt some bytes")
@async_run_until_complete
async def step_impl(context: Context):
    assert_that(hasattr(context, "host"), equal_to(True), "Encrypter service host not set")
    assert_that(hasattr(context, "port"), equal_to(True), "Encrypter service port not set")
    assert_that(hasattr(context, "value"), equal_to(True), "String to encrypt not set")

    async with _Client(host=context.host, port=context.port) as client:
        encoded = b64encode(context.value.encode("utf-8"))
        value = await client.encrypt_bytes(encoded)
        assert_that(len(value), not_(equal_to(0)), "Encrypted value of zero length")
        context.encrypted = value


@step("Decrypt the encrypted string back to bytes")
@async_run_until_complete
async def step_impl(context):
    assert_that(hasattr(context, "host"), equal_to(True), "Encrypter service host not set")
    assert_that(hasattr(context, "port"), equal_to(True), "Encrypter service port not set")
    assert_that(hasattr(context, "value"), equal_to(True), "String to encrypt not set")

    async with _Client(host=context.host, port=context.port) as client:
        value = await client.decrypt_bytes(context.encrypted)
        assert_that(b64decode(value).decode("utf-8"), equal_to(context.value), "Decrypted value not same as original")
